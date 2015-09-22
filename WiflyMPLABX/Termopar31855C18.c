// https://dl.dropboxusercontent.com/u/101922388/WiflySanUSB.zip
// Videos: https://www.youtube.com/watch?v=bN7zj7BQXnE
// APK: https://play.google.com/store/apps/details?id=appinventor.ai_sandro_juca.Wifi_SanUSB
// http://www.youtube.com/watch?v=gUe24X1gNVw

#include "SanUSB48.h"
/***************************************************************************
Firmware and Front-end: https://dl.dropboxusercontent.com/u/101922388/WiflySanUSB.zip
APK: https://play.google.com/store/apps/details?id=appinventor.ai_sandro_juca.Wifi_SanUSB
Video http://www.youtube.com/watch?v=gUe24X1gNVw

After uploading or reset, wait until only the green led on the Wifly modem is blinking and click connect on the front-end.
Após o upload ou reset, aguarde ate que somente o led verde do modem Wifly esteja piscando e clique em conectar no front-end

Se for a primeira gravação do PIC com modem conectado, talvez seja necessário resetar o microcontrolador após a gravação, retirando e colocando o cabo USB novamente.
//Ao digitar 192.168.1.195/YT na barra de endereço de qualquer navegador (browser), o sinal do LED será comutado (toggle) e aparecerá uma página HTML.

 The microcontroller can be, with this firmware, server (switching the led signal through 192.168.1.195/YT in adress bar) or also client (the AD converter value is inserted in google databank).
 **********************************************************************************/
//*
 const rom char pg[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<center><iframe src=\"https://docs.google.com/spreadsheet/formResponse?"
                      "formkey=dDZwVlR4RG5sTW5Nelo2RlNPVEQ2Ymc6MQ&ifq%20&entry.0.single=";

 const rom char pg1[] ="&entry.1.single=";

 const rom char pg2[] = "&submit=Submit\"></iframe><H1>Join tinyurl.com/SanUSB</H1><P></H1>Access <a href=\"https://docs.google.com/spreadsheet/"
                         "ccc?key=0Ap332OAhnMe2dDZwVlR4RG5sTW5Nelo2RlNPVEQ2Ymc\">Logs</a></P></center>\r\n";
 
#pragma udata
char rede[] = "SanUSB";       // Your WiFi SSID name
char senha[] = "silveir3";    // Your WiFi password
char ip[] = "192.168.1.195";  // Set intranet static IP (DHCP OFF). In this the case the static IP .195 is based on the gateway IP 192.168.1.1.
                              // To check the IP of your gateway, type ipconfig or ifconfig at the prompt or terminal on the computer
                              // Default Wifly RN-XV SanUSB IP: 192.168.1.195 Port: 80. The Wifly IP must be xxx.xxx.x.1xx , with "1" in last byte for the gateway IP.

short int pisca=0, AT1=0,  AT2=0, start=0, flagsw=0;
char  comando[64], n=0, m=0;
unsigned int i=0;
float temp = 0, ADvalue = 0;
char str[10];

#define TC_CLK pin_c0 
#define TC_CS pin_c1
#define TC_DATA entrada_pin_c2 //PORTCbits.RC2 -Default entrada -> TRISC=1

void FloattoAscSerial(float value, char *string)
 {
     if (value < 0) {// se valor for negativo
         *string++ = '-';
         value = -value;
     }
 sprintf(string, (const far rom char *) "%lu.%02u",
                     (long) value,//int Decimal = diferenca do float e a parte inteira (long)
                     (int) ((value - (long) value) * 100));

    sendsw((char *)str);
  }

unsigned long int TempSPI(void)
    {
    unsigned long int valor = 0;
    char i;

     nivel_baixo(TC_CS); //pára a conversão AD para transmitir
    for (i=0;i<32;i++) 
        {
        nivel_alto(TC_CLK); //tempo_ms(1);
        if (TC_DATA==1){valor = valor | tmp;}
        nivel_baixo(TC_CLK); //tempo_ms(1);
        if (i != 31) valor = valor << tmp;
        }
    nivel_alto(TC_CS);
    valor = 0x1FFF & (valor >> 18); //shift 18 e retorna ate 13 bits válidos do AD
    temp = valor * 0.25; //Converte para graus
     }

#pragma interrupt interrupcao //Tem que estar aqui ou dentro do firmware.c
void interrupcao(){
if (serial_interrompeu)  {
     serial_interrompeu=0;
     comando[n] = le_serial(); m=n; //comand[m]=comando[n];

         switch (comando[n]){

             case 'C':
             AT1=1;
             break;

             case 'I':
             AT2=1;
             break;

             case 'A':
             AT1=1;
             break;

             case 'S':
             AT1=1;
             break;

             case '*':
             AT1=1;
             break;
           case 'Y':
             {n=0;comando[0] = 'Y';}
              break;
         }
 //***************possibility 1->character***********************
                            if (comando[0]== 'Y' && n==1){comando[0]== 32;
                            //if (comando[n-1]== 'Y'){comando[0]== 32;
                                       switch(RCREG)
                                                  {
                                                case 'L':  nivel_alto(pin_b7); //type in browser address bar: 192.168.1.195/YL
                                                           flagsw=1;
                                                break;

                                                case 'D': nivel_baixo(pin_b7); //type in browser address bar: 192.168.1.195/YD
                                                break;

                                                case 'P':  nivel_alto(pin_b7); //type in browser address bar: 192.168.1.195/YP
                                                break;

                                                case 'T': inverte_saida(pin_b7); //type in browser address bar: 192.168.1.195/YT to toggle Led and open a HTML page
                                                          flagsw=1;
                                                break;

                                                 }
                                                         }
//***************possibility 2-> word**************************
                            if ( comando[1]== 'O' && comando[2]== 'N')
                                                {
                                                 nivel_alto(pin_b7);  // ON
                                                }

                            if ( comando[1]== 'O' && comando[2]== 'F' && comando[3]== 'F')
                                                {

                                                 nivel_baixo(pin_b7); // OFF
                                                }
         ++n; if(n>=64){n=0;}
   }

                   }

#include "confws.h"

void main(void)
{
    clock_int_48MHz();
    habilita_interrupcao(recep_serial);
    taxa_serial(9600);
    habilita_canal_AD(AN0);
    iniws();
    //habilita_wdt();
    
 for (i=0;i<8;i++) 
        {
        tempo_ms(1000);
        }
    while(1)
    {   //ClrWdt();
         flagsw=1;
 for (i=0;i<8;i++) 
        {
        tempo_ms(1000);
        }

           if (flagsw==1) {flagsw=0;

                           ADvalue=(2*4.3*le_AD10bits(0))/1023;
                           tempo_ms(1000);

                           sendrw((rom char*)pg);
                           TempSPI();
                           FloattoAscSerial(temp,str);
                           
                           sendrw((rom char *)pg1);
                           
                           FloattoAscSerial(ADvalue,str);
                           
                           sendrw((rom char*)pg2);
                           tempo_ms(1000);


                         }
    }
}
     
