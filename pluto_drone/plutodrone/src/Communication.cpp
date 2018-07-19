#include "ros/ros.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <plutodrone/Communication.h>

#include <time.h>
//#include <plutodrone/Protocol.h>
//#include <stdlib.h>


#include <unistd.h>
//#include <plutodrone/Protocol.h>
//#include <string>
//#include <plutodrone/Communication.h>



std::string MSP_HEADER="$M<";



using namespace std;


//Protocol pro;





int optval;
socklen_t optlen = sizeof(optval);

/*uint8_t Communication::checksum=0;
uint8_t Communication::recbuf[1024];

 int Communication::c_state = IDLE;
     uint8_t c;
     bool err_rcvd = false;
     int offset = 0, dataSize = 0;
     uint8_t cmd;
*/





bool Communication::connectSock(std::string ip)
{



  const char *ip_addr=ip.c_str();
  int res;
  struct sockaddr_in addr;
  long arg;
  fd_set myset;
  struct timeval tv;
  int valopt;
  socklen_t lon;

  cout<<"Connecting to Pluto......\n";

  // Create socket
  sockID = socket(AF_INET, SOCK_STREAM, 0);
  if (sockID < 0) {
    // fprintf(stderr, "Error creating socket (%d %s)\n", errno, strerror(errno));
     cout<<"Cannot connect to Pluto, please try again\n";

     exit(0);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(23);
  addr.sin_addr.s_addr = inet_addr(ip_addr);

  // Set non-blocking
  if( (arg = fcntl(sockID, F_GETFL, NULL)) < 0) {
    // fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
   cout<<"Cannot connect to Pluto, please try again\n";

     exit(0);
  }
  arg |= O_NONBLOCK;
  if( fcntl(sockID, F_SETFL, arg) < 0) {
    // fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
    cout<<"Cannot connect to Pluto, please try again\n";

     exit(0);
  }
  // Trying to connect with timeout
  res = connect(sockID, (struct sockaddr *)&addr, sizeof(addr));


 //fprintf(stderr,"socket is ready %d",res);
  if (res < 0) {
     if (errno == EINPROGRESS) {
  //      fprintf(stderr, "socket is ready %d ",res);
    //    fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
        do {
           tv.tv_sec = 7;
           tv.tv_usec = 0;
           FD_ZERO(&myset);
           FD_SET(sockID, &myset);
           res = select(sockID+1, NULL, &myset, NULL, &tv);
 //fprintf(stderr,"socket is ready %d",res);

           if (res < 0 && errno != EINTR) {
            //  fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
              cout<<"Cannot connect to Pluto, please try again\n";
              exit(0);
           }
           else if (res > 0) {
              // Socket selected for write
              lon = sizeof(int);
              if (getsockopt(sockID, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) {
              //   fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno));
                 cout<<"Cannot connect to Pluto, please try again\n";
                 exit(0);
              }
              // Check the value returned...
              if (valopt) {
                // fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt));
                cout<<"Cannot connect to Pluto, please try again\n";

                 exit(0);
              }
              break;
           }
           else {
            //  fprintf(stderr, "Timeout in select() - Cancelling!\n");
             cout<<"Cannot connect to Pluto, please try again\n";
              exit(0);
           }
        } while (1);
     }
     else {
        //fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
         cout<<"Cannot connect to Pluto, please try again\n";
        exit(0);
     }
  }
  // Set to blocking mode again...
  if( (arg = fcntl(sockID, F_GETFL, NULL)) < 0) {
    // fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
     cout<<"Cannot connect to Pluto, please try again\n";
      exit(0);
  }
  arg &= (~O_NONBLOCK);
  if( fcntl(sockID, F_SETFL, arg) < 0) {
    // fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
     cout<<"Cannot connect to Pluto, please try again\n";
     exit(0);
  }


  // I hope that is all



//printf("Hello message sent\n");

 /* Check the status for the keepalive option */
   if(getsockopt(sockID, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
    //  perror("getsockopt()");
      cout<<"Cannot connect to Pluto, please try again\n";
      close(sockID);
      exit(EXIT_FAILURE);
   }
  // printf("SO_KEEPALIVE is %s\n", (optval ? "ON" : "OFF"));

   /* Set the option active */
   optval = 1;
   optlen = sizeof(optval);
   if(setsockopt(sockID, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
    //  perror("setsockopt()");
      cout<<"Cannot connect to Pluto, please try again\n";
      close(sockID);
      exit(EXIT_FAILURE);
   }
   //printf("SO_KEEPALIVE set on socket\n");

   /* Check the status again */
   if(getsockopt(sockID, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
    //  perror("getsockopt()");
      cout<<"Cannot connect to Pluto, please try again\n";
      close(sockID);
      exit(EXIT_FAILURE);
   }
   //printf("SO_KEEPALIVE is %s\n", (optval ? "ON" : "OFF"));


int error = 0;
socklen_t len = sizeof (error);
int retval = getsockopt (sockID, SOL_SOCKET, SO_ERROR, &error, &len);


if (retval != 0) {
    /* there was a problem getting the error code */
  //  fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
    cout<<"Cannot connect to Pluto, please try again\n";
    exit(EXIT_FAILURE);
   // return;
}


if (error != 0) {
    /* socket has a non zero error status */
  //  fprintf(stderr, "socket error: %s\n", strerror(error));

   cout<<"Cannot connect to Pluto, please try again\n";
    exit(EXIT_FAILURE);
}

else

{


//fprintf(stderr, "socket is up \n");

cout<<"Pluto Connected\n";

}


return true;



}



int Communication::writeSock(const void *buf, int count)
{



// while (Communication::socketSyckLock) {
//   /* code */
// //printf("value of synclock in write = %i\n",Communication::socketSyckLock );
//   usleep(2);
// }



  //usleep(2000);
 int k=write(sockID,buf,count);
 //Communication::socketOpStarted=1;

//usleep(500);
 //readFrame();
Communication::socketSyckLock=1;
return k;

}



uint8_t Communication::readSock(void *buf, int count)

{




int k=read(sockID,buf,count);

if(k>0)
{

uint8_t val=Communication::recbuf[0];
  return val;

}

else
return k;


}



// void Communication::readFrame()
//
// {
//
//
//
//
//
//
//
//                  len = 0;
//                  Communication::checksum=0;
//                  command=0;
//                  payload_size=0;
//                  checksumIndex=5;
//
//                  while (!Communication::socketSyckLock) {
//                    /* code */
//                 //   printf("value of synclock in read = %i\n",Communication::socketSyckLock );
//                    usleep(2);
//                  }
//
//                 usleep(1000);
//                  len = readSock(buf,1024);
//
//                  if(len > 0) {
//                   //   NSMutableData* data=[[NSMutableData alloc] initWithLength:0];
//
//                     // [data appendBytes: (const void *)buf length:len];
//
//                      // NSString *s = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
//
//                      printf("buffred read length= %i\n",len );
//
//
//                      char c=buf[0];
//
//                      if(buf[0]=='$'&&buf[1]=='M'&&buf[2]=='>')
//                      {
//
//
//                          payload_size=(buf[3] & 0xFF);
//                          command=(buf[4] & 0xFF);
//                          Communication::checksum^=(payload_size & 0xFF);
//                          Communication::checksum^=(command & 0xFF);
//                          indx=0;
//
//  //
//                           printf("####### Recevied Packet MSP NO =%i\n",command);
//                           printf("####### Recevied Packet Payload Size=%i\n",payload_size);
//
//                          for(int i=5;i<len-1;i++)
//                          {
//
//                              uint8_t k=(int8_t)(buf[i] & 0xFF);
//
//                               inputBuffer[indx++]=k;
//                            //   NSLog(@"####### value of =%i",inputBuffer[indx-1]);
//
//
//                              Communication::checksum^=(buf[i] & 0xFF);
//                              checksumIndex++;
//
//                          }
//
//                        if((Communication::checksum & 0xFF)==(buf[checksumIndex]&0xFF))
//                         {
//                            printf("####### valid packet\n");
//
//                           bufferIndex=0;
//                         if(payload_size>0)
//
//                         pro.evaluateCommand(command);
//
//                         }
//
//
//                      }
//
//
//                   //  printf("Reading in the following:\n");
//                   // printf("%s", buf);
//
//                     // [self readIn:s];
//
//                   //   [data release];
//                  }
//
//
//                 Communication::socketSyckLock=0;
//
// }






 void Communication::readFrame()
 {



              Communication::c = readSock(Communication::recbuf,1);
            //  Log.v("READ", "Data: " + c);



          //  printf("read Value= %i\n",c );
            //  Communication::c_state = IDLE;
            //  Log.e("MultiwiiProtocol", "Read  = null");


          if (Communication::c_state == IDLE) {
              Communication::c_state = (Communication::c == '$') ? HEADER_START : IDLE;
          } else if (Communication::c_state == HEADER_START) {
              Communication::c_state = (Communication::c == 'M') ? HEADER_M : IDLE;
          } else if (Communication::c_state == HEADER_M) {
              if (Communication::c == '>') {
                  Communication::c_state = HEADER_ARROW;
              } else if (Communication::c == '!') {
                  Communication::c_state = HEADER_ERR;
              } else {
                  Communication::c_state = IDLE;
              }
          } else if (Communication::c_state == HEADER_ARROW || Communication::c_state == HEADER_ERR) {
      /* is this an error message? */
              Communication::err_rcvd = (Communication::c_state == HEADER_ERR); /*
                         * now we are expecting the
                         * payload size
                         */
              Communication::dataSize = (Communication::c & 0xFF);
      /* reset index variables */
            //  p = 0;
              Communication::offset = 0;
              Communication::checksum = 0;
              Communication::checksum ^= (Communication::c & 0xFF);
      /* the command is to follow */
              Communication::c_state = HEADER_SIZE;
          } else if (Communication::c_state == HEADER_SIZE) {
              Communication::cmd = (uint8_t) (Communication::c & 0xFF);
//printf("cmd Value= %i\n",cmd );

              Communication::checksum ^= (Communication::c & 0xFF);
              Communication::c_state = HEADER_CMD;
          }  else if (Communication::c_state == HEADER_CMD && Communication::offset < Communication::dataSize) {
              Communication::checksum ^= (Communication::c & 0xFF);
              Communication::inputBuffer[Communication::offset++] = (uint8_t) (Communication::c & 0xFF);

              if(Communication::cmd==108)
              {


                //  Log.d("#########", "MSP_ATTITUDE: recived payload= "+inBuf[offset-1]);


              }
          } else if (Communication::c_state == HEADER_CMD && Communication::offset >= Communication::dataSize) {
      /* compare calculated and transferred Communication::checksum */
              if ((Communication::checksum & 0xFF) == (Communication::c & 0xFF)) {
                  if (Communication::err_rcvd) {
                    //  Log.e("Multiwii protocol",
                      //        "Copter did not understand request type " + c);
                  } else {
          /* we got a valid response packet, evaluate it */
                      //SONG BO HERE WE RECEIVED ENOUGH DATA-----------------------
                    //  evaluateCommand(cmd, (int) dataSize);

                          Communication::bufferIndex=0;
  //                          printf("cmd Value= %i\n",cmd );
                         evaluateCommand(Communication::cmd);

                      //SONG BO ---------------------------------------
                    //  DataFlow = DATA_FLOW_TIME_OUT;
                  }
              } else {
                  // Log.e("Multiwii protocol", "invalid Communication::checksum for command "
                  //         + ((int) (cmd & 0xFF)) + ": " + (Communication::checksum & 0xFF)
                  //         + " expected, got " + (int) (c & 0xFF));
                  // Log.e("Multiwii protocol", "<" + (cmd & 0xFF) + " "
                  //         + (dataSize & 0xFF) + "> {");
                  // for (i = 0; i < dataSize; i++) {
                  // if (i != 0) {
                  // Log.e("Multiwii protocol"," ");
                  // }
                  // Log.e("Multiwii protocol",(inBuf[i] & 0xFF));
                  // }
                  //Log.e("Multiwii protocol", "} [" + c + "]");
                  //Log.e("Multiwii protocol", new String(inBuf, 0, dataSize));
              }
              Communication::c_state = IDLE;
}

}







//  Protocol.cpp content

int Communication::read8()
{

 return Communication::inputBuffer[Communication::bufferIndex++] & 0xff;



}


int Communication::read16()
{


      int add_1=(Communication::inputBuffer[Communication::bufferIndex++] & 0xff) ;
      int add_2=((Communication::inputBuffer[Communication::bufferIndex++]) << 8);

      return add_1+add_2;




}


int Communication::read32()
{

      int add_1=(Communication::inputBuffer[Communication::bufferIndex++] & 0xff);
      int add_2=((Communication::inputBuffer[Communication::bufferIndex++] ) << 8);
      int add_3=((Communication::inputBuffer[Communication::bufferIndex++] ) << 16);
      int add_4=((Communication::inputBuffer[Communication::bufferIndex++] ) << 24);
  
  return add_1+add_2+add_3+add_4;
 /* return (Communication::inputBuffer[Communication::bufferIndex++] & 0xff) + ((Communication::inputBuffer[Communication::bufferIndex++] & 0xff) << 8)+ ((Communication::inputBuffer[Communication::bufferIndex++] & 0xff) << 16) + ((Communication::inputBuffer[Communication::bufferIndex++] & 0xff) << 24);*/


}


void Communication::evaluateCommand(int command)
{






      switch (command) {


         case MSP_FC_VERSION:

              FC_versionMajor=read8();
              FC_versionMinor=read8();
              FC_versionPatchLevel=read8();

      //          printf("FC_versionMajor= %i\n",FC_versionMajor);
        //          printf("FC_versionMinor= %i\n",FC_versionMinor);
          //          printf("FC_versionPatchLevel= %i\n",FC_versionPatchLevel);

              break;

          case MSP_RAW_IMU:


              Communication::accX=read16();
              Communication::accY=read16();
              Communication::accZ=read16();


//              printf("accX %f\n",accX);
  //          printf("accY %f\n",accY);
    //        printf("accZ %f\n",accZ);



              Communication::gyroX=read16()/8;
              Communication::gyroY=read16()/8;
              Communication::gyroZ=read16()/8;




              Communication::magX=read16()/3;
              Communication::magY=read16()/3;
              Communication::magZ=read16()/3;




              break;


          case MSP_ATTITUDE:

             // for(int i=0; i<6;i++)
                //  NSLog(@"value of %i",inputBuffer[i]);



              Communication::roll=(read16()/10);
              Communication::pitch=(read16()/10);
              Communication::yaw=read16();

              // printf("Roll %i\n",roll);
               //printf("Pitch %i\n",pitch);
               //printf("Yaw %i\n",yaw);

              break;


         case MSP_ALTITUDE:

              Communication::alt=(read32()/10);

            //  printf("Altitude = %i\n",alt);
              break;

         case MSP_ANALOG:


              Communication::battery=(read8()/10.0);

          //   printf("Battery Volt %f\n",battery);
        //     printf("meter sum %i\n",read16());
              Communication::rssi=read16();
      //        printf("Rssi %i\n",rssi);
            //  NSLog(@"amperage %i",[MultiWi230 read16]);
  //  printf("Dummy value %i\n",read16());
    //    printf("Dummy value %i\n",read16());

              break;



          case MSP_ACC_TRIM:

              Communication::trim_pitch=read16();


          //printf("TrimPitch %i\n",trim_pitch);

              Communication::trim_roll=read16();


          //printf("TrimRoll %i\n",trim_roll);


              break;


              case MSP_RC:
                             Communication::rcRoll = read16();

                            //  printf("rcRoll %i\n",rcRoll);

                             Communication::rcPitch = read16();

                              //printf("rcPitch %i\n",rcPitch);
                             Communication::rcYaw = read16();

                             //printf("rcYaw %i\n",rcYaw);
                             Communication::rcThrottle = read16();

                             //printf("rcThrottle %i\n",rcThrottle);
                             Communication::rcAUX1 = read16();

                             //printf("rcAUX1 %i\n",rcAUX1);
                             Communication::rcAUX2 = read16();

                             //printf("rcAUX2 %i\n",rcAUX2);
                             Communication::rcAUX3 = read16();

                             //printf("rcAUX3 %i\n",rcAUX3);
                             Communication::rcAUX4 = read16();

                             //printf("rcAUX4 %i\n",rcAUX4);

                             // cout<<"\nsockID:"<<sockID;//<<"  Yaw:"<<rcYaw<<" Roll:"<<rcRoll<<" Pitch:"<<rcPitch;
                             // cout<<" Throttle:"<<rcThrottle<<" AUX1:"<<rcAUX1<<" AUX2:"<<rcAUX2;
                              //cout<<" AUX3:"<<rcAUX3<<" AUX4:"<<rcAUX4;
                             //cout<<"time:"<<time(0);
                             
                             /*gettimeofday(&tp, NULL);
                             c_t=tp.tv_sec*1000000L+tp.tv_usec;
                             cout<<"  "<<c_t;
                             p_t=c_t;*/
                             /*count++;
                             if (time(0)-saved_time)
                              count=0;
                             saved_time=time(0);
                             cout<<" "<<count;*/


             break;

          default:
              break;












}

}

void Communication::sendRequestMSP(std::vector<int8_t> data)
{




writeSock(&data[0],data.size());
//gettimeofday(&tp, NULL);
//c_t=tp.tv_sec*1000000L+tp.tv_usec;
//cout<<c_t<<endl;




}


std::vector<int8_t>  Communication::createPacketMSP(int msp, std::vector<int8_t>payload)
{




  if (msp < 0) {
            // return NULL;
         }
        std::vector<int8_t> bf;


     for(std::string::iterator it = MSP_HEADER.begin(); it != MSP_HEADER.end(); ++it) {

       bf.push_back((int8_t) (*it & 0xFF));

       }

         int8_t checksum = 0;
         int8_t pl_size = (int8_t) ((!payload.empty() ? (int) (payload.size()) : 0) & 0xFF);
         bf.push_back(pl_size);
         checksum ^= (pl_size & 0xFF);

         bf.push_back((int8_t) (msp & 0xFF));
         checksum ^= (msp & 0xFF);

//printf("MSP No %d\n",msp );

//printf("Payload size %d\n",payload.size() );

         if (!payload.empty()) {
//printf("Adding payload %d\n");

             for (std::vector<int8_t>::iterator it = payload.begin() ; it != payload.end(); ++it) {

                int8_t k=*it;
  //             printf("payload  value %i\n",k);
                 bf.push_back((int8_t) (k & 0xFF));
                 checksum ^= (k & 0xFF);
             }
         }
         bf.push_back(checksum);
         
         //for (std::vector<int8_t>::const_iterator i = bf.begin(); i != bf.end(); ++i)
         //std::cout << *i << ' ';
         
         
         return (bf);



}


void Communication::sendRequestMSP_SET_RAW_RC(int channels[])
{

std::vector<int8_t>rc_signals(16);

int index = 0;
       for (int i = 0; i < 8; i++) {
           //Log.d("drona", "Data: " + (channels8[i]));
           rc_signals[index++] = (int8_t) (channels[i] & 0xFF);
           rc_signals[index++] = (int8_t) ((channels[i] >> 8) & 0xFF);

        //   printf("rcvalue = %i\n" ,channels[i]);
          // rc_signals.push_back((uint8_t) (channels[i] & 0xFF));
           //rc_signals.push_back( (uint8_t) ((channels[i] >> 8) & 0xFF));
       }



// printf("size of rc_array %d\n",rc_signals.size() );
 sendRequestMSP(createPacketMSP(MSP_SET_RAW_RC, rc_signals));


}


void Communication::sendRequestMSP_SET_POS(int posArray[])
{

std::vector<int8_t>posData(8);

int index = 0;
       for (int i = 0; i < 4; i++) {
           //Log.d("drona", "Data: " + (channels8[i]));
           posData[index++] = (int8_t) (posArray[i] & 0xFF);
           posData[index++] = (int8_t) ((posArray[i] >> 8) & 0xFF);

      //  printf("posvalue = %i\n" ,posArray[i]);
          // rc_signals.push_back((uint8_t) (channels[i] & 0xFF));
           //rc_signals.push_back( (uint8_t) ((channels[i] >> 8) & 0xFF));
       }



// printf("size of rc_array %d\n",rc_signals.size() );
 sendRequestMSP(createPacketMSP(MSP_SET_POS, posData));


}





void Communication::sendRequestMSP_GET_DEBUG(std::vector<int> requests)
{

for (size_t i = 0; i < requests.size(); i++) {

 {sendRequestMSP(createPacketMSP(requests[i], std::vector<int8_t>()));
 }
}

}
