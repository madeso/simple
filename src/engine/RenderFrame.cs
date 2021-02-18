using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Tao.Platform.Windows;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Tao.OpenGl;

namespace SimpleEngine
{
    public class RenderFrame : Form
    {
        private IntPtr hDC;
        private IntPtr hRC;
        private bool active = true;
        private bool fullscreen = true;

        private RenderFrame()
        {
            this.CreateParams.ClassStyle = this.CreateParams.ClassStyle | User.CS_HREDRAW | User.CS_VREDRAW | User.CS_OWNDC;
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.DoubleBuffer, true);
            this.SetStyle(ControlStyles.Opaque, true);
            this.SetStyle(ControlStyles.ResizeRedraw, true);
            this.SetStyle(ControlStyles.UserPaint, true);

            this.Activated += new EventHandler(this.Form_Activated);
            this.Deactivate += new EventHandler(this.Form_Deactivate);
            this.KeyDown += new KeyEventHandler(this.Form_KeyDown);
            this.KeyUp += new KeyEventHandler(this.Form_KeyUp);
            this.MouseUp += new MouseEventHandler(RenderFrame_MouseUp);
            this.MouseDown += new MouseEventHandler(RenderFrame_MouseDown);
            this.MouseWheel += new MouseEventHandler(RenderFrame_MouseWheel);
        }

        public bool Active
        {
            get
            {
                return active;
            }
        }

        public System.Drawing.Point Center
        {
            get
            {
                return new System.Drawing.Point(this.Width / 2, this.Height / 2);
            }
        }  

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                kill();
            }
            base.Dispose(disposing);
        }

        // prevent stupid flickering
        protected override void OnPaintBackground(PaintEventArgs e)
        {
        }

        public static RenderFrame Create(string title, int width, int height, int bits, bool fullscreen)
        {
            int pixelFormat;

            // force a swap
            GC.Collect();
            Kernel.SetProcessWorkingSetSize(Process.GetCurrentProcess().Handle, -1, -1);

            if (fullscreen)
            {
                Gdi.DEVMODE dmScreenSettings = new Gdi.DEVMODE();
                dmScreenSettings.dmSize = (short)Marshal.SizeOf(dmScreenSettings);
                dmScreenSettings.dmPelsWidth = width;
                dmScreenSettings.dmPelsHeight = height;
                dmScreenSettings.dmBitsPerPel = bits;
                dmScreenSettings.dmFields = Gdi.DM_BITSPERPEL | Gdi.DM_PELSWIDTH | Gdi.DM_PELSHEIGHT;

                if (User.ChangeDisplaySettings(ref dmScreenSettings, User.CDS_FULLSCREEN) != User.DISP_CHANGE_SUCCESSFUL)
                {
                    // fullscreen not supported
                    return null;
                }
            }

            RenderFrame form = new RenderFrame();
            form.FormBorderStyle = FormBorderStyle.None;

            form.Width = width;
            form.Height = height;
            form.Text = title;
            form.fullscreen = fullscreen;

            Gdi.PIXELFORMATDESCRIPTOR pfd = new Gdi.PIXELFORMATDESCRIPTOR();
            pfd.nSize = (short)Marshal.SizeOf(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = Gdi.PFD_DRAW_TO_WINDOW |
                Gdi.PFD_SUPPORT_OPENGL |
                Gdi.PFD_DOUBLEBUFFER;
            pfd.iPixelType = (byte)Gdi.PFD_TYPE_RGBA;
            pfd.cColorBits = (byte)bits;
            pfd.cRedBits = 0;
            pfd.cRedShift = 0;
            pfd.cGreenBits = 0;
            pfd.cGreenShift = 0;
            pfd.cBlueBits = 0;
            pfd.cBlueShift = 0;
            pfd.cAlphaBits = 0;
            pfd.cAlphaShift = 0;
            pfd.cAccumBits = 0;
            pfd.cAccumRedBits = 0;
            pfd.cAccumGreenBits = 0;
            pfd.cAccumBlueBits = 0;
            pfd.cAccumAlphaBits = 0;
            pfd.cDepthBits = 16;
            pfd.cStencilBits = 0;
            pfd.cAuxBuffers = 0;
            pfd.iLayerType = (byte)Gdi.PFD_MAIN_PLANE;
            pfd.bReserved = 0;
            pfd.dwLayerMask = 0;
            pfd.dwVisibleMask = 0;
            pfd.dwDamageMask = 0;

            form.hDC = User.GetDC(form.Handle);
            if (form.hDC == IntPtr.Zero)
            {
                form.kill();
                throw new UserException("Can't Create A GL Device Context.");
            }

            pixelFormat = Gdi.ChoosePixelFormat(form.hDC, ref pfd);
            if (pixelFormat == 0)
            {
                form.kill();
                throw new UserException("Can't Find A Suitable PixelFormat.");
            }

            if (!Gdi.SetPixelFormat(form.hDC, pixelFormat, ref pfd))
            {
                form.kill();
                throw new UserException("Can't Set The PixelFormat.");
            }

            form.hRC = Wgl.wglCreateContext(form.hDC);
            if (form.hRC == IntPtr.Zero)
            {
                form.kill();
                throw new UserException("Can't Create A GL Rendering Context.");
            }

            if (!Wgl.wglMakeCurrent(form.hDC, form.hRC))
            {
                form.kill();
                throw new UserException("Can't Activate The GL Rendering Context.");
            }

            form.Show();
            form.TopMost = true;
            form.Focus();

            if (fullscreen)
            {
                Cursor.Hide();
            }
            else
            {
                Cursor.Show();
            }

#if DEBUG
            form.Location = new System.Drawing.Point(0, 0);
#endif

            return form;
        }

        public void kill()
        {
            if (fullscreen)
            {
                User.ChangeDisplaySettings(IntPtr.Zero, 0);
                Cursor.Show();
            }

            if (hRC != IntPtr.Zero)
            {
                if (!Wgl.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero))
                {
                    MessageBox.Show("Release Of DC And RC Failed.", "SHUTDOWN ERROR",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                if (!Wgl.wglDeleteContext(hRC))
                {
                    MessageBox.Show("Release Rendering Context Failed.", "SHUTDOWN ERROR",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                hRC = IntPtr.Zero;
            }

            if (hDC != IntPtr.Zero)
            {
                if (!IsDisposed)
                {
                    if (Handle != IntPtr.Zero)
                    {
                        if (!User.ReleaseDC(Handle, hDC))
                        {
                            MessageBox.Show("Release Device Context Failed.", "SHUTDOWN ERROR",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                }

                hDC = IntPtr.Zero;
            }

            Hide();
            Close();
        }

        private void Form_Activated(object sender, EventArgs e)
        {
            active = true;
        }

        private void Form_Deactivate(object sender, EventArgs e)
        {
            active = false;
        }

        private void Form_KeyDown(object sender, KeyEventArgs e)
        {
            events.Add(new Event(true, new Ckey(e.KeyCode)));
        }

        private void Form_KeyUp(object sender, KeyEventArgs e)
        {
            events.Add(new Event(false, new Ckey(e.KeyCode)));
        }

        public struct Event
        {
            public Event(bool down, Ckey button)
            {
                this.button = button;
                this.down = down;
            }

            public override string ToString()
            {
                return string.Format("{0} -> {1}", button, down);
            }

            public readonly Ckey button;
            public readonly bool down;
        }

        private int delta = 0;
        void RenderFrame_MouseWheel(object sender, MouseEventArgs e)
        {
            const int WHEEL_DELTA = 120;
            delta += e.Delta;
            while (delta >= WHEEL_DELTA)
            {
                delta -= WHEEL_DELTA;
                events.Add(new Event( true, Ckey.WheelUp ));
                events.Add(new Event( false, Ckey.WheelUp ));
            }
            while (delta <= -WHEEL_DELTA)
            {
                delta += WHEEL_DELTA;
                events.Add(new Event(true, Ckey.WheelDown));
                events.Add(new Event(false, Ckey.WheelDown));
            }
        }

        void RenderFrame_MouseDown(object sender, MouseEventArgs e)
        {
            events.Add(new Event(true, new Ckey(e.Button)));
        }

        void RenderFrame_MouseUp(object sender, MouseEventArgs e)
        {
            events.Add(new Event(false, new Ckey(e.Button)));
        }

        List<Event> events = new List<Event>();

        public new IEnumerable<Event> Events
        {
            get
            {
                foreach (Event e in events)
                    yield return e;
                events.Clear();
            }
        }

        public void swap()
        {
            Gdi.SwapBuffers(hDC);
        }

        public void setCursorPos(System.Drawing.Point p)
        {
            Cursor.Position = p;
        }
        public System.Drawing.Point getCursorPos()
        {
            return Cursor.Position;
        }

        public void begin()
        {
            Setup.Viewport(Rect.FromLTWH(0,0,Width, Height));
            Gl.glClear(Gl.GL_COLOR_BUFFER_BIT | Gl.GL_DEPTH_BUFFER_BIT);
        }
    }
}
