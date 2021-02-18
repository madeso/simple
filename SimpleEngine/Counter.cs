using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace SimpleEngine
{
    public class Counter
    {
        [DllImport("Kernel32.dll")]
        private static extern bool QueryPerformanceCounter(
            out long lpPerformanceCount);

        [DllImport("Kernel32.dll")]
        private static extern bool QueryPerformanceFrequency(
            out long lpFrequency);

        private long startTime, stopTime;
        private long freq;

        // Constructor

        public Counter()
        {
            startTime = 0;
            stopTime = 0;

            if (QueryPerformanceFrequency(out freq) == false)
            {
                // high-performance counter not supported
                throw new UserException("Hi-performance counter not supported");
            }
        }

        public void Start()
        {
            //Thread.Sleep(0);
            QueryPerformanceCounter(out startTime);
        }

        public void Stop()
        {
            QueryPerformanceCounter(out stopTime);
        }

        public double DurationD
        {
            get
            {
                return (double)(stopTime - startTime) / (double)freq;
            }
        }

        public float Duration
        {
            get
            {
                return (float)DurationD;
            }
        }
    }
}