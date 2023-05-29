#pragma once
#include "Common.h"

namespace PBD
{
    class TimeManager
    {
    private:
        Real time;
        static TimeManager* current;
        Real h;
    public:
        TimeManager();
        ~TimeManager();

        // 单例
        static TimeManager* getCurrent();
        static void setCurrent(TimeManager* tm);
        static bool hasCurrent();

        Real getTime();
		void setTime(Real t);
		Real getTimeStepSize();
		void setTimeStepSize(Real tss);
    };
    

    
}