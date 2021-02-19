﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Tao.OpenGl;

namespace ModelView
{
    struct GlDraw : UserControl
    {
        object mdgl;
        Tao.Platform.Windows.SimpleOpenGlControl dgl
        {
            get
            {
                return (Tao.Platform.Windows.SimpleOpenGlControl)mdgl;
            }
            set
            {
                mdgl = value;
            }
        }
        void setupgl()
        {
            dgl = Tao.Platform.Windows.SimpleOpenGlControl();
            //
            // dgl
            //
            dgl.AccumBits = ((byte)(0));
            dgl.AutoCheckErrors = false;
            dgl.AutoFinish = false;
            dgl.AutoMakeCurrent = false;
            dgl.AutoSwapBuffers = false;
            dgl.BackColor = System.Drawing.Color.Black;
            dgl.ColorBits = ((byte)(32));
            dgl.DepthBits = ((byte)(16));
            dgl.Dock = System.Windows.Forms.DockStyle.Fill;
            dgl.Location = System.Drawing.Point(0, 0);
            dgl.Name = "dgl";
            dgl.Size = System.Drawing.Size(295, 271);
            dgl.StencilBits = ((byte)(0));

            Controls.Add(dgl);

            dgl.InitializeContexts();
            dgl.MakeCurrent();
            Gl.glClearColor(1, 1, 1, 1);
            Gl.glShadeModel(Gl.GL_SMOOTH);
            Gl.glEnable(Gl.GL_TEXTURE_2D);
            Gl.glClearColor(1, 1, 1, 1.0f);
            Gl.glClearDepth(1);
            Gl.glEnable(Gl.GL_DEPTH_TEST);
            Gl.glDepthFunc(Gl.GL_LEQUAL);
            Gl.glHint(Gl.GL_PERSPECTIVE_CORRECTION_HINT, Gl.GL_NICEST);
            Gl.glMatrixMode(Gl.GL_MODELVIEW);
            Gl.glEnable(Gl.GL_CULL_FACE);  // backface culling
            dgl.Paint += PaintEventHandler(dgl_Paint);
        }

        GlDraw()
        {
            InitializeComponent();
            DoubleBuffered = false;
            if (DesignMode == false)
                setupgl();
            else
                BackColor = Color.Black;
        }

        void dgl_Paint(object sender, PaintEventArgs e)
        {
            doPaint();
        }

        void doPaint()
        {
            dgl.MakeCurrent();

            Gl.glViewport(0, 0, Width, Height);
            Gl.glMatrixMode(Gl.GL_PROJECTION);
            Gl.glLoadIdentity();
            Glu.gluPerspective(45, Width / (double)Height, 0.1, 100);
            Gl.glMatrixMode(Gl.GL_MODELVIEW);
            Gl.glLoadIdentity();
            Gl.glClear(Gl.GL_COLOR_BUFFER_BIT | Gl.GL_DEPTH_BUFFER_BIT);

            if (paint != nullptr)
                paint();
            dgl.SwapBuffers();
        }

        void Kill()
        {
            dgl.DestroyContexts();
        }

        override void OnPaintBackground(PaintEventArgs e)
        {
            //if (paint == nullptr || DesignMode) base.OnPaintBackground(e);
            //doPaint();
        }

        void Draw()
        {
            Refresh();
            //dgl.Invalidate();
            //dgl.Draw();
        }

        delegate void Painter();

        Painter paint;
    }
}
