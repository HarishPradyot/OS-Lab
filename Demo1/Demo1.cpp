#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 

using namespace std;
int main(int argc,char *argv[])
{
    
    cout<<"OS Demo 1"<<endl;
    cout<<"_____________________________________________\n"<<endl;

    
    int readFD[3];
    string files[]={"./X.txt", "./Y.txt", "./Z.txt"};
    
    // Opening X, Y and Z files
    for(int i=0;i<3;i++)
        readFD[i]=open(&files[i][0],O_RDONLY);
    
    
    char *readStream=new char[1];
    int bytesRead=0;
    
    
    // If the current user has the permission to read the file, then the file descriptor returned by open function will be >-1
    // Checking if the current user/group has permissions on the files "X.txt", "Y.txt" and "Z.txt" respectively
    for(int i=0;i<3;i++)
    {
        if(readFD[i]>-1)
        {
            bytesRead=read(readFD[i],readStream,sizeof(char));
            if(bytesRead>0)
                cout<<"Contents of "<<files[i]<<" is "<<readStream<<endl;
        }
        close(readFD[i]);
    }
    cout<<"_____________________________________________"<<endl;


    
    // If the current user has the permission to write to the file, then the file descriptor returned by open function will be >-1
    // Only the user 'A' has permissions to write to the Files "X.txt", "Y.txt" and "Z.txt"  
    int writeFD[3];
    for(int i=0;i<3;i++)
        writeFD[i]=open(&files[i][0],O_WRONLY);
    
    char *writeStream=new char[1];
    // Only give access to modify if the Write Permission has been given for atleast one file
    bool fileWriteAccess=(writeFD[0]>-1||writeFD[1]>-1||writeFD[2]>-1);

    bool isRunning=true;
    while(isRunning&&fileWriteAccess)
    {
        cout<<"\n____________________________________________"<<endl;
        // Write to respective file based on choice
        cout<<"\nPress and the enter the following keys for the given activity: \n";
        for(int i=0;i<3;i++)
            if(writeFD[i]>-1)
                cout<<"  "<<i+1<<" to change contents of file "<<files[i].substr(2)<<endl;
        
        cout<<"  4 to exit the Program\n\n";

        int option;
        cin>>option; 
        cin.ignore();  
        if(option!=4)
        {
            cout<<"Enter the value to replace contents of the file "<<files[option-1].substr(2)<<" : "<<endl;
            cin>>writeStream;
        }
        
        
        // Changing the files based on the write permissions and selection from the above options
        if(option==4)
        {
            cout<<"Exiting..."<<endl;
            isRunning=false;
            continue;
        }
        for(int i=0;i<3;i++)
        {
            if(option==i+1)
            {
                if(writeFD[i]>-1)
                {
                    lseek(writeFD[i],0,SEEK_SET);
                    write(writeFD[i],writeStream,sizeof(char));
                }
                else
                    cout<<"Cannot access file. Select correct option";
            }
        }       
        cout<<"_____________________________________________"<<endl;
    }
    
    
    for(int i=0;i<3;i++)
        close(writeFD[i]);
    return 0;
}