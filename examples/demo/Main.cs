using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Tao.OpenGl;

namespace SimpleTest
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
            dgl.InitializeContexts();
            vgl();
            Gl.glShadeModel(Gl.GL_SMOOTH);                                      // Enable Smooth Shading
            Gl.glClearColor(0, 0, 0, 0.5f);                                     // Black Background
            Gl.glClearDepth(1);                                                 // Depth Buffer Setup
            Gl.glEnable(Gl.GL_DEPTH_TEST);                                      // Enables Depth Testing
            Gl.glDepthFunc(Gl.GL_LEQUAL);                                       // The Type Of Depth Testing To Do
            Gl.glHint(Gl.GL_PERSPECTIVE_CORRECTION_HINT, Gl.GL_NICEST);  
            vgl();
        }

        private void vgl()
        {
            int error = Gl.glGetError();
            if (error != Gl.GL_NO_ERROR)
            {
                throw new Exception("GL error");
            }
        }

        private void dgl_Paint(object sender, PaintEventArgs e)
        {
            vgl();
            Gl.glClear(Gl.GL_COLOR_BUFFER_BIT | Gl.GL_DEPTH_BUFFER_BIT);        // Clear Screen And Depth Buffer
            Gl.glLoadIdentity();                                                // Reset The Current Modelview Matrix
            Gl.glTranslatef(-1.5f, 0, -6);                                      // Move Left 1.5 Units And Into The Screen 6.0
            Gl.glBegin(Gl.GL_TRIANGLES);                                        // Drawing Using Triangles
            Gl.glVertex3f(0, 1, 0);                                         // Top
            Gl.glVertex3f(-1, -1, 0);                                       // Bottom Left
            Gl.glVertex3f(1, -1, 0);                                        // Bottom Right
            Gl.glEnd();                                                         // Finished Drawing The Triangle
            Gl.glTranslatef(3, 0, 0);                                           // Move Right 3 Units
            Gl.glBegin(Gl.GL_QUADS);                                            // Draw A Quad
            Gl.glVertex3f(-1, 1, 0);                                        // Top Left
            Gl.glVertex3f(1, 1, 0);                                         // Top Right
            Gl.glVertex3f(1, -1, 0);                                        // Bottom Right
            Gl.glVertex3f(-1, -1, 0);                                       // Bottom Left
            Gl.glEnd();   
            vgl();
            Gl.glFlush();
            vgl();
            dgl.SwapBuffers();

        }

        private void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            dgl.DestroyContexts();
        }
    }
}
