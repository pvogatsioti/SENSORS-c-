#include<iostream.h>
#include<stdlib.h>
#include<string.h>
#include <ctime>
/*------------------------------------------------------------------------------------
* This is a program that simulates what is written below.                            *
*                                                                                    *
* In a forest area which is a rectangular with coordinates from (0,0) to (1600,1600) *
* 10 fire sensors are placed randomly. Every sensor has some characterestics. These  *
* are the coordinates of the area in which is placed, the polution percentage in the *
* area and the limit above of the polution becomes dangerous.                        *
*                                                                                    *
* Every 5 minutes each sensor measures the polution in the area which is placed. If  *
* it is above the limit, it sends the data to a central system which prints in the   *
* screen the coordinates, the percentage of the poluted areas and the average        *
* polution in that area.   
*							                                                         *
**
*------------------------------------------------------------------------------------*/

//=================================================================================================================================
//=================================================================================================================================
//======================================    FIRST PART  ===========================================================================
class CentralSystem{
    private:
        float A[3][10]; //the array that holds in the first row the polution result of every sensor(10 sensors)
                        // 2nd row the x coordinate 3rd row y coordinate
        
    public:
        static int j; // is used for the columns of the Array, it is static because it increments every time a sensor sends a result
        CentralSystem();
        ~CentralSystem();
        void receive_data(float pol,int *cor);
        void print_data();
        void init_array();
    
};
int CentralSystem::j=0;


CentralSystem::CentralSystem(){
    int c, r;
    
    for(c=0;c<3;c++)
        for(r=0;r<10;r++)
            A[c][r]=0.0;
   
}

CentralSystem::~CentralSystem()
{
    
    delete []A;
}

void CentralSystem::init_array()
{
     int c, r;
    
    for(c=0;c<3;c++)
        for(r=0;r<10;r++)
            A[c][r]=0.0;
}
void CentralSystem::receive_data(float pol,int *cor){
    
    if(j<10) 
    {   
        A[0][j] = pol;
        
        A[1][j] = cor[0];
        
        A[2][j] = cor[1];    
    }
     
    j++;   
}
void CentralSystem::print_data(){
    int i,c;
    float sum=0.0,avg;
    
    for(i=0;i<3;i++){
        for(c=0;c<j;c++)
            {
                if(i==0 && c==0)
                    cout<<"Polution  ";
                if(i==1 && c==0)
                    cout<<"coord X   ";
                if(i==2 && c==0)
                    cout<<"coord Y   ";
                cout<<A[i][c]<<"     ";
            }
        cout<<endl;
    }
    for(c=0;c<j;c++)
    {
        sum+=A[0][c];
        
    }
    avg = sum/(float)j;
    
    cout<<"Average="<<avg<<endl;
}
class Sensor{
    private:
        //char *brand;
        //int coord[2];
        //float polution;
        float limit;
        CentralSystem *sys;
    public:
        char *brand;
        int coord[2];
        float polution;
        Sensor(char *name,int x, int y,float lim,CentralSystem *cesy);
        ~Sensor();
        virtual void measure_polution(); // when we call the function if it is NOK sensor it has to call NOK's function
        void check_new_measure();
        virtual void measure_humidity(){}
        virtual void check_humid(){}
        virtual void print_humid(){}
        
};

Sensor::Sensor(char *name,int x, int y,float lim,CentralSystem *cesy){
    
    coord[0]=x;
    coord[1]=y;
    brand = new char[strlen(name)+1];
    strcpy(brand,name);
    limit=lim;
    sys = cesy;
    
}

Sensor::~Sensor(){
    
    delete brand;
    delete sys;
    
    
}
void Sensor:: measure_polution(){
    
    polution =(rand()%100)/100.0;
    //return polution;
   
}

void Sensor::check_new_measure(){
    if(polution>=limit){
        cout<<" DANGER ";
        sys->receive_data(polution,coord);
    }
}
//=========================================================================================================================
//=========================================================================================================================
//=========================================================================================================================
//=========================================================================================================================
class NOK:public Sensor {
    private:
        static float const pol_error = 0.001;
    public:
        NOK(char *name,int x, int y,float lim,CentralSystem *cesy);
        ~NOK();
        void measure_polution();
            
        
};
NOK::NOK(char *name,int x, int y,float lim,CentralSystem *cesy):Sensor(name,x, y,lim,cesy){
   cout<<"NOK Constructor"<<endl;
}
void NOK::measure_polution(){
    
    this->Sensor::measure_polution();
    polution = polution - pol_error*polution;
}
class FiremenGroup{
    private:
        char *chief;
        char *group[7];
        int A[2];
    public:
        void receive_assignment(int *B);
};
void FiremenGroup::receive_assignment(int *B)
{   
    A[0]=B[0];
    A[1]=B[1];
    cout<<"patrol in the area: ("<<A[0]<<","<<A[1]<<")"<<endl;
}
class FireDepartment{
    private:
        
        FiremenGroup fg;
        float humid_array[3][5];
    public:
        static int jj;
        void receive_data(float hmd,int *cord);
        void assign_firemen_group(int *crd);
        FireDepartment();
        
};
int FireDepartment::jj = 0;
FireDepartment::FireDepartment()
{
    cout<<"epiasa fwtiaaa"<<endl;
    int c,r;
    for(r=0;r<3;r++)
        for(c=0;c<5;c++)
            humid_array[r][c]=0.0;
}           
void FireDepartment::assign_firemen_group(int *crd)
{   
    fg.receive_assignment(crd);
    
}
void FireDepartment::receive_data(float hmd,int *cord)
{
    
    humid_array[0][jj]=hmd;
    
    humid_array[1][jj]=cord[0];
    humid_array[2][jj]=cord[1];
    
    assign_firemen_group(cord);
    
    jj++;
    
    
}

class ERIC:public Sensor{
    private:  
        //float humidity;
        float limit_hum;
        FireDepartment *fide;
    public:
        float humidity;
        ERIC(FireDepartment *fde,char *name,float max_humid,int x, int y,float lim,CentralSystem *cesy);
        ~ERIC();
        void measure_humidity();
        void check_humid();
        void print_humid(){ cout<<" humidity--->"<<humidity;}
        
};

ERIC::ERIC(FireDepartment *fde,char *name, float max_humid,int x, int y,float lim,CentralSystem *cesy):Sensor(name,x,y,lim,cesy){
    cout<<"ERIC Constructor"<<endl;
    limit_hum = max_humid;
    fide=fde;
}
void ERIC::measure_humidity(){
    humidity =(rand()%100)/100.0;
    
}
void ERIC::check_humid(){
    if(humidity<limit_hum){
        cout<<" Low Humdity"<<endl;
        
        fide->receive_data(humidity,coord);
    }
}

//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************

main(){
    int minutes,i,x,y,z,counter=0;
    Sensor *Area[1600][1600];
    CentralSystem *cs;
    cs= new CentralSystem;
    NOK *pa;
    FireDepartment *NYFD;
    NYFD = new FireDepartment;
    
    cout<<endl<<endl<<"In a forest area which is a rectangular with coordinates from (0,0) to (1600,1600)"<<endl;
    cout<<"10 fire sensors are placed randomly. "<<endl;
    cout<<"Every 5 minutes each sensor measures the polution in the area which is placed."<<endl;
    cout<<"If it is above the limit (0.5) it sends the data to a central system"<<endl;
    cout<<"which prints in the screen the coordinates, the percentage of the poluted areas and the average"<<endl;
    cout<<"polution in that area."<<endl<<endl;
    
    cout<<"Time in minutes in which the simulator will run"<<endl<<"Time:";
    cin>>minutes;
    cout<<endl;
    for(x=0;x<1600;x++)
        for(y=0;y<1600;y++)
            {
                Area[x][y]=NULL;
            }
    srand(time(NULL));
    for(i=0;i<5;i++){// random cordinates of the NOK sensor
        x=rand()%1601;
        z=rand()%1601;
        
        Area[x][z]=new NOK("NOK",x,z,0.5,cs);
        
        
        
    }
    for(i=0;i<5;i++){ // random cordinates of the ERIC sensor
        x=rand()%1601;
        z=rand()%1601;
        
        Area[x][z]=new ERIC(NYFD,"ERIC",0.5,x,z,0.5,cs);
        
        
        
    }
    
    
    for(i=1;i<=minutes;i++){
        if((i%5)==0){ // each repetition is considered a minute
            
            cs->init_array();
            cs->j=0;
            NYFD->jj=0;
            counter++;
            cout<<endl<<"Sensors sent results "<<counter<<" time(s)"<<endl<<endl;
            for(x=0;x<1600;x++)
                for(y=0;y<1600;y++)
                {
                    if (Area[x][y]!=NULL){
                        
                        
                        Area[x][y]->measure_polution();
                        cout<<Area[x][y]->brand<<" ("<<Area[x][y]->coord[0]<<","<<Area[x][y]->coord[1]<<") polution---->"<<Area[x][y]->polution;
                        Area[x][y]->check_new_measure();
                        
                        
                        pa = dynamic_cast <NOK*> (Area[x][y]);
                        if(pa==0) // returns 0 only if it can not make the change ==> ERIC Sensor
                        {
                            
                            Area[x][y]->measure_humidity();
                            Area[x][y]->print_humid();
                            Area[x][y]->check_humid();
                            
                           
                            
                        }
                        cout<<endl;
                     }
                }
            cout<<endl;
            cout<<endl<<"CS Screen"<<endl<<endl;
            cs->print_data();
        }
    }
    
    delete cs;
    for(x=0;x<1600;x++)
                for(y=0;y<1600;y++){
                    if (Area[x][y]!=NULL)
                        delete Area[x][y];
                }
}





