#pragma once
namespace PBD
{
    class IDFactory
    {
    private:
        static int id;
    public:
        static int getId(){
            return id;
        }
    }; 
}