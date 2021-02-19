using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace SimpleEngine
{
    struct Counter
    {
        [DllImport("Kernel32.dll")] static extern bool QueryPerformanceCounter(
            out long lpPerformanceCount);

        [DllImport("Kernel32.dll")] static extern bool QueryPerformanceFrequency(
            out long lpFrequency);

        long startTime, stopTime;

        long freq;

        // Constructor

        Counter()
        {
            startTime = 0;
            stopTime = 0;

            if (QueryPerformanceFrequency(out freq) == false)
            {
                // high-performance counter not supported
                throw UserException("Hi-performance counter not supported");
            }
        }

        void Start()
        {
            //Thread.Sleep(0);
            QueryPerformanceCounter(out startTime);
        }

        void Stop()
        {
            QueryPerformanceCounter(out stopTime);
        }

        double DurationD
        {
            get
            {
                return (double)(stopTime - startTime) / (double)freq;
            }
        }

        float Duration
        {
            get
            {
                return (float)DurationD;
            }
        }
    }
}
