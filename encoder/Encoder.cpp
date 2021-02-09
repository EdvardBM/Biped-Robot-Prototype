 
// Language dependencies
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <iostream>

// POSIX dependencies
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 

#define eQEP0 "/sys/devices/platform/ocp/48300000.epwmss/48300180.eqep"
#define eQEP1 "/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep"
#define eQEP2 "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep"

 
// Constructor for eQEP driver interface object
class Encoder
{
 public :  
    Encoder(int EncoderNumber)
    {  
        switch (EncoderNumber)
        {
            case 0:
            path = eQEP0;
            break;
            case 1:
            path = eQEP1;
            break;
            case 2:
            path = eQEP2;
            break;
            default :
            std::cout << "invalid Encoder number" << std::endl;
            break;
        } 
    }
    void initEncoder (int32_t pos, int32_t period)
    {
        setMode();
        setPeriod(period);
        setPosition(pos);
    }
    
    int32_t getPosition()
    {  
        int32_t       position;
        FILE* fp = fopen((this->path + "/position").c_str(), "r");

        if(fp == NULL)
        {  
                std::cerr << "[eQEP " << this->path << "] Unable to open position for read" << std::endl;
                return 0;
        }

        fscanf(fp, "%d", &position);
        fclose(fp);

        return position;
    }

 private :
    std::string path;

    void setMode()
    {  
        FILE* setmode = fopen((this->path + "/mode").c_str(), "w");
        if(setmode == NULL)
        {
            std::cerr << "[eQEP " << this->path << "] Unable to open mode for write" << std::endl;
            return;
        }
        fprintf(setmode, "%u\n", 0); //absolute mode
        fclose(setmode);
    }

    void setPosition(int32_t position)
    {  
        
        FILE *setpos = fopen((this->path + "/position").c_str(), "w");
        if(setpos == NULL)
        {
            std::cerr << "[eQEP " << this->path << "] Unable to open position for write" << std::endl;
            return;
        }
        fprintf(setpos, "%d\n", position);
        fclose(setpos);
    }  


    void setPeriod(uint64_t period)
    {
        FILE* setp = fopen((this->path + "/period").c_str(), "w");
        if(setp == NULL)
        {
            std::cerr << "[eQEP " << this->path << "] Unable to open period for write" << std::endl;
            return;
        }


        fprintf(setp, "%llu\n", period);
        fclose(setp);
    }
};

int main ()
{
    Encoder encoder(2); //encoder number 2
    encoder.initEncoder(0, 10000000L);
    
    while(1)
    {
        std::cout << encoder.getPosition()*0.03 <<std::endl;
        printf("\033[2J\033[1;1H");
    }
    std::cin.get();
    return 0;
}
