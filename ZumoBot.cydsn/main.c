/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>
/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/



#if 0
// Hello World!
void zmain(void)
{
    printf("\nHello, World!\n");
    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
 }   
#endif

#if 0
// Name and age
void zmain(void)
{
    char name[32];
    int age;
    
    printf("\n\n");
    
    printf("Enter your name: ");
    //fflush(stdout);
    scanf("%s", name);
    printf("Enter your age: ");
    //fflush(stdout);
    scanf("%d", &age);
    
    printf("You are [%s], age = %d\n", name, age);

    while(true)
    {
        BatteryLed_Write(!SW1_Read());
        vTaskDelay(100);
    }
 }   
#endif

#if 0
//battery level//
void zmain(void)
{
    ADC_Battery_Start();        

    int16 adcresult =0;
    float volts = 0.0;

    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed

    while(true)
    {
        char msg[80];
        ADC_Battery_StartConvert(); // start sampling
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            
            // Print both ADC results and converted value
            printf("%d %f\r\n",adcresult, volts);
        }
        vTaskDelay(500);
    }
 }   
#endif

#if 0
// button
void zmain(void)
{
    while(true) {
        printf("Press button within 5 seconds!\n");
        int i = 50;
        while(i > 0) {
            if(SW1_Read() == 0) {
                break;
            }
            vTaskDelay(100);
            --i;
        }
        if(i > 0) {
            printf("Good work\n");
            while(SW1_Read() == 0) vTaskDelay(10); // wait until button is released
        }
        else {
            printf("You didn't press the button\n");
        }
    }
}
#endif

#if 0
// button
void zmain(void)
{
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    while(true)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) vTaskDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif

#if 0
//ultrasonic sensor//
void zmain(void)
{
    Ultra_Start();                          // Ultra Sonic Start function
    
    while(true) {
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
         print_mqtt("Zumo039/ready", "%d", d);
        vTaskDelay(20);
    }
}   
#endif

#if 0
//IR receiverm - how to wait for IR remote commands
void zmain(void)
{
    IR_Start();
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    bool led = false;
    // Toggle led when IR signal is received
    while(true)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        if(led) printf("Led is ON\n");
        else printf("Led is OFF\n");
    }    
 }   
#endif

#if 0
//IR receiver - read raw data
void zmain(void)
{
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    // print received IR pulses and their lengths
    while(true)
    {
        if(IR_get(&IR_val, portMAX_DELAY)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
        }
    }    
 }   
#endif

#if 0
//reflectance
void zmain(void)
{
    struct sensors_ ref;
    struct sensors_ dig;

    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    

    while(true)
    {
        // read raw sensor values
        reflectance_read(&ref);
        // print out each period of reflectance sensors
        print_mqtt("Zumo039/lol", "%d %d %d %d %d %d", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       

        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig); 
        //print out 0 or 1 according to results of reflectance period
        printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);        
        
        vTaskDelay(200);
    }
}   
#endif

#if 0
//motor
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors

    vTaskDelay(3000);
    
    motor_forward(100,5000);     // moving forward
    motor_turn(200,50,2000);     // turn
    motor_turn(50,200,2000);     // turn
    motor_backward(100,2000);    // moving backward
     
    motor_forward(0,0);         // stop motors

    motor_stop();               // disable motor controller
    
    while(true)
    {
        vTaskDelay(100);
    }
}
#endif

#if 0
/* Example of how to use te Accelerometer!!!*/
    void zmain(void)
{
    struct accData_ data;
    
    printf("Accelerometer test...\n");
    motor_start();
    motor_forward(0,0);
    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    
    while(true)
    {
        motor_forward(255,0);
        LSM303D_Read_Acc(&data);
        print_mqtt("Zumo039/acc", "%d", data.accX);
    }
 }   
#endif    

#if 0
// MQTT test
void zmain(void)
{
    int ctr = 0;

    printf("\nBoot\n");
    send_mqtt("Zumo039/debug", "Boot");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 

    while(true)
    {
        printf("Ctr: %d, Button: %d\n", ctr, SW1_Read());
        print_mqtt("Zumo039/debug", "Ctr: %d, Button: %d", ctr, SW1_Read());

        vTaskDelay(1000);
        ctr++;
    }
 }   
#endif

#if 0
void zmain(void)
{    
    struct accData_ data;
    struct sensors_ ref;
    struct sensors_ dig;
    
    printf("MQTT and sensor test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Accelerometer Ok...\n");
    }
    
    int ctr = 0;
    reflectance_start();
    while(true)
    {
        LSM303D_Read_Acc(&data);
        // send data when we detect a hit and at 10 second intervals
        if(data.accX > 1500 || ++c,,,,,tr > 1000) {
            printf("Acc: %8d %8d %8d\n",data.accX, data.accY, data.accZ);
            print_mqtt("Zumo01/acc", "%d,%d,%d", data.accX, data.accY, data.accZ);
            reflectance_read(&ref);
            printf("Ref: %8d %8d %8d %8d %8d %8d\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       
            print_mqtt("Zumo01/ref", "%d,%d,%d,%d,%d,%d", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);
            reflectance_digital(&dig);
            printf("Dig: %8d %8d %8d %8d %8d %8d\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            print_mqtt("Zumo01/dig", "%d,%d,%d,%d,%d,%d", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            ctr = 0;
        }
        vTaskDelay(10);
    }
 }   

#endif

#if 0
void zmain(void)
{    
    RTC_Start(); // start real time clock
    
    RTC_TIME_DATE now;

    // set current time
    now.Hour = 12;
    now.Min = 34;
    now.Sec = 56;
    now.DayOfMonth = 25;
    now.Month = 9;
    now.Year = 2018;
    RTC_WriteTime(&now); // write the time to real time clock

    while(true)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            printf("%2d:%02d.%02d\n", now.Hour, now.Min, now.Sec);
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }   
#endif

// SOS - DONE!
#if 0
// SOS
void batteryled_dot() // batteryLED dot function
{
    BatteryLed_Write(1); vTaskDelay(500);
    BatteryLed_Write(0); vTaskDelay(500);
}
void batteryled_dash() // batteryLED dash function
{
    BatteryLed_Write(1); vTaskDelay(1500);
    BatteryLed_Write(0); vTaskDelay(500);
}

void zmain(void)
{
    int x;
    while(true)
    {
        if(SW1_Read() == 0)
        {
            for(x=0; x < 9; x++)
            {
                if(x < 3)
                {
                    batteryled_dot(); 
                }
                else if(x < 6)
                {
                    batteryled_dash();
                }
                else
                {
                    batteryled_dot();
                }
            } 
        }
    }
}

#endif

//battery level - DONE!
#if 0
//battery level 
void warningled(float volts)
    {
        if (volts < 4)
            {
                
                while(SW1_Read() == 1)
                {
                    BatteryLed_Write(1);
                    vTaskDelay(100);
                    BatteryLed_Write(0);
                    vTaskDelay(100);
                }
                BatteryLed_Write(0);
            }
    }
void zmain(void)
{
    ADC_Battery_Start();        

    int16 adcresult =0;
    float volts = 0.0;

    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed

    while(true)
    {
        ADC_Battery_StartConvert(); // start sampling
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) // wait for ADC converted value
        {   
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            volts = adcresult / 4095.0 * 5.0 * 1.75 ;  // convert value to Volts'
            // Print both ADC results and converted value
            printf("%d %f\r\n",adcresult, volts);
            warningled(volts);
        }
        
        vTaskDelay(500);
    }
 }   
#endif

//race course without sensors - DONE!
#if 0
//race course without sensors
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors
    vTaskDelay(2000);
    motor_forward(200,1800);    // forward
    motor_turn(200,10,420);    // 1st turn: turn right
    motor_forward(200,1200);    // forward
    motor_turn(200,10,470);    // 2nd turn: turn right
    motor_forward(200,1600);    // forward
    motor_turn(200,10,400);    // 3rd turn: curve forward*/
    motor_turn(200,150,2200);    // 3rd turn: curve forward*/
    motor_forward(0,0);         // set speed to zero to stop motors
    motor_stop();               // disable motor controller
    
    while(true)
    {
        vTaskDelay(100);
    }
}
#endif

//motor with ultrasonic distance sensor - DONE!
#if 0
//motor with ultrasonic distance sensor
void zmain(void)
{
    Ultra_Start();                          // Ultra Sonic Start function
    motor_start();
    
    while(true)
    {
        motor_forward(150, 0);
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
        printf("distance = %d\r\n", d);
        if(d < 10)
        {
            motor_forward(0,0);
            Beep(500, 100);
            motor_backward(200, 100);
            motor_turn(50, 200, 1200);
            // motor_backward(200, 500);
        }
    }
}   
#endif

// motor with accelerometer - DONE!
#if 0
// motor with accelerometer
void zmain(void)
{
    struct accData_ data;
    int rng;
    
    printf("Accelerometer test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    
    motor_start();              // enable motor controller
    motor_forward(0,0);
    vTaskDelay(1500);
    motor_forward(200,0);

    while(true)
    {
        rng = rand() % 2;
        if (rng == 1)
        {
            motor_turn(200,100,200);
        }
        else
        {
            motor_turn(100,200,200);
        }
        LSM303D_Read_Acc(&data);
        if(data.accX < -495)
        {
            printf("%8d %8d %8d\n",data.accX, data.accY, data.accZ);
            rng = rand() % 2;
            if (rng == 1)
            {
                motor_backward(200,200);
                motor_turn(200,100,300);
                motor_forward(200,0);
            }
            else
            {
                motor_backward(200,200);
                motor_turn(100,200,300);
                motor_forward(200,0);
            }
        }
    }
 }   
#endif  

//motor straight line + four lines with reflectance sensors - DONE!
#if 0
//motor straight line + four lines with reflectance sensors
void zmain(void)
{
    struct sensors_ dig;
    int lines = 0;
    IR_Start();
    IR_flush(); // clear IR receive buffer
    
    reflectance_start();
    reflectance_set_threshold(9300, 8400, 7800, 7800, 8000, 8600);
    motor_start();
    motor_forward(0,0);
    while(SW1_Read() == 1);
    vTaskDelay(1500);
    motor_forward(200,0);
    
    while(lines == 0)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            IR_wait();
            motor_forward(200,0);
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            lines++;
        }
    }
    while(lines < 3)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            lines++;
        }
    }
    while(lines == 3)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            motor_stop();
            lines++;
        }
    }
        
    while(true)
    {

        vTaskDelay(100);
    }
}   
#endif

//motor intersections with reflectance sensors - DONE!
#if 0
//motor intersections with reflectance sensors
    
void motor_tankright (uint8 speed,uint32 delay) // turn right while immobile
{
    MotorDirLeft_Write(0);      // set LeftMotor backward mode
    MotorDirRight_Write(1);     // set RightMotor forward mode
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    vTaskDelay(delay);
}
void motor_tankleft (uint8 speed,uint32 delay) // turn left while immobile
{
    MotorDirLeft_Write(1);      // set LeftMotor forward mode
    MotorDirRight_Write(0);     // set RightMotor backward mode
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    vTaskDelay(delay);
}

void zmain(void)
{
    struct sensors_ dig; int lines = 0;
    IR_Start(); IR_flush(); // clear IR receive buffer
    
    reflectance_start();
    reflectance_set_threshold(13300, 8400, 7800, 7800, 8000, 12600);
    motor_start();
    motor_forward(0,0);
    while(SW1_Read() == 1);
    vTaskDelay(1500);
    motor_forward(200,0);
    
    while(lines == 0)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            vTaskDelay(1500);    //IR_wait();
            motor_forward(200,0);            
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            lines++;
        }
    }
    
    while(lines < 4)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            if(lines == 1)
            {
                motor_forward(100, 225);
                motor_tankleft(100,700); // 90 degree turn (left)
                motor_forward(100, 50);
                lines++;
            }
            else if(lines < 4)
            {
                motor_forward(100, 275);
                motor_tankright(100,760); // 90 degree turn (right)
                motor_forward(100, 50);
                lines++;
            }
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(35,100,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(100,35,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(65,100,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(100,65,0); 
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(0,100,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(100,0,0);
        }
    }
    
    while(lines == 4)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(100, 225);
            motor_forward(0,0);
            motor_stop();
            lines++;
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(100,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(50,100,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(100,50,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(75,100,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(100,75,0); 
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(25,100,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(100,25,0);
        }
    }
        
    while(true)
    {
        vTaskDelay(100);
    }
}   
#endif 

//motor course with reflectance sensors - DONE!
#if 0
//motor course with reflectance sensors
void zmain(void)
{
    struct sensors_ dig;
    int lines = 0;
    IR_Start();
    IR_flush(); // clear IR receive buffer
    
    reflectance_start();
    reflectance_set_threshold(8300, 7400, 6800, 6800, 7000, 7600);
    motor_start();
    motor_forward(0,0);
    while(SW1_Read() == 1);
    vTaskDelay(1500);
    motor_forward(200,0);
    
    while(lines == 0)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            IR_wait();
            motor_forward(200,0);            
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            lines++;
        }
    }
    while(lines == 1)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            motor_stop();
            lines++;
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(200,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(35,200,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(200,35,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(95,200,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(200,95,0); 
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(20,180,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(180,20,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 1) // turn super tight left
        {
            motor_turn(0,120,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn super tight right
        {
            motor_turn(120,0,0);
        }
    }
        
    while(true)
    {
        vTaskDelay(100);
    }
}   
#endif

// sending time to MTQQ - DONE!
#if 0
// sending time to MTQQ
void zmain(void)
{    
    RTC_Start(); // start real time clock
    RTC_TIME_DATE current_time;
    int minutes, hours;
    
    // set current time
    current_time.Sec = 0;
    current_time.DayOfMonth = 3;
    current_time.Month = 10;
    current_time.Year = 2019;
    printf("Enter current hour(s):");
    scanf("%2d", &hours);
    current_time.Hour = hours;
    printf("\nEnter current minute(s):");
    scanf("%2d", &minutes);
    current_time.Min = minutes;
    RTC_WriteTime(&current_time); // write the time to real time clock*/
    
    while(true)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            current_time = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */
            print_mqtt("Zumo037/time", "%2d:%02d", current_time.Hour, current_time.Min);    //Send message to topic
            printf("%2d:%02d.%02d\n", current_time.Hour, current_time.Min, current_time.Sec);
            while(SW1_Read() == 0) vTaskDelay(50);
        }
    }
 }   
#endif

//MQTT ultrasonic distance - DONE!
#if 0
//MQTT ultrasonic distance
void zmain(void)
{
    Ultra_Start();                          // Ultra Sonic Start function
    motor_start();
    int rng;
    
    while(true)
    {
        int d = Ultra_GetDistance();    // Print the detected distance (centimeters)
        motor_forward(200, 0);
        if(d < 10)
        {
            motor_backward(200, 100);
            rng = rand() % 2;
            if(rng == 1)
            {
                motor_turn(200, 0, 500);
                print_mqtt("Zumo037/turn", "Right turn");
            }
            else;
            {
                motor_turn(0, 200, 500);
                print_mqtt("Zumo037/turn", "Left turn");
            }
        }
    }
}   
#endif

//MTQQ intersections - DONE!
#if 0
//MTQQ intersections
void zmain(void)
{
    TickType_t start; TickType_t end;
    struct sensors_ dig;
    int lines = 0;
    IR_Start(); IR_flush(); // clear IR receive buffer
    
    reflectance_start();
    reflectance_set_threshold(7300, 6400, 5800, 5800, 6000, 6600);
    motor_start();
    motor_forward(0,0);
    while(SW1_Read() == 1);
    vTaskDelay(1500);
    motor_forward(200,0);
    
    while(lines == 0)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            IR_wait();
            IR_flush();
            start = xTaskGetTickCount();
            motor_forward(200,0);
            lines++;
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            
        }
    }
        
    while(true)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            end = xTaskGetTickCount();
            motor_forward(0,0);
            print_mqtt("Zumo037/lap", "%dms", end - start);
            IR_wait();
            IR_flush();
            start = xTaskGetTickCount();
            motor_forward(200,0);
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
        }
    }
}   
#endif

// ROBOT PROJECTS

// Sumo Wrestling - DONE!
#if 0
// Sumo Wrestling
void motor_tankright (uint8 speed,uint32 delay) // tankturn right
{
    MotorDirLeft_Write(0);      // set LeftMotor backward mode
    MotorDirRight_Write(1);     // set RightMotor forward mode
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    vTaskDelay(delay);
}
void motor_tankleft (uint8 speed,uint32 delay) // tankturn left
{
    MotorDirLeft_Write(1);      // set LeftMotor forward mode
    MotorDirRight_Write(0);     // set RightMotor backward mode
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    vTaskDelay(delay);
}
void edgeturn() // randomize turn(L/R) and angle (120-180 degrees)
{
    int turn, rng;
    motor_backward(200,230);
    turn = rand() % 2; // decide left or right
    rng = rand() % 7; // decide turn angle
    if(turn == 0) // TURN RIGHT
    {
        if(rng == 0) // 180 degree turn
        {
            motor_tankright(200,541);
        }
        else if(rng == 1) // 170 degree turn
        {
            motor_tankright(200,519);
        }
        else if(rng == 2) // 160 degree turn
        {
            motor_tankright(200,497);
        }
        else if(rng == 3) // 150 degree turn
        {
            motor_tankright(200,462);
        }
        else if(rng == 4) // 140 degree turn
        {
            motor_tankright(200,428);
        }
        else if(rng == 5) // 130 degree turn
        {
            motor_tankright(200,399);
        }
        else if(rng == 6) // 120 degree turn
        {
            motor_tankright(200,350);
        }
    }
    else // TURN LEFT
    {
        if(rng == 0) // 180 degree turn
        {
            motor_tankleft(200,530);
        }
        else if(rng == 1) // 170 degree turn
        {
            motor_tankleft(200,509);
        }
        else if(rng == 2) // 160 degree turn
        {
            motor_tankleft(200,483);
        }
        else if(rng == 3) // 150 degree turn
        {
            motor_tankleft(200,461);
        }
        else if(rng == 4) // 140 degree turn
        {
            motor_tankleft(200,419);
        }
        else if(rng == 5) // 130 degree turn
        {
            motor_tankleft(200,384);
        }
        else if(rng == 6) // 120 degree turn
        {
            motor_tankleft(200,340);
        }
    }
    motor_forward(255,0);
}
void startup(TickType_t *start) // startup + the first line
{
    struct sensors_ dig; int lines = 0;
    motor_start(); motor_forward(0,0);
    while(SW1_Read() == 1); // press button to go to first line
    vTaskDelay(1500);
    motor_forward(255,0);
    
    while(lines == 0)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1) // straight line
        {
            motor_forward(0,0);
            print_mqtt("Zumo039/ready", "zumo");
            IR_wait(); // vTaskDelay(1000);    
            *start = xTaskGetTickCount();
            print_mqtt("Zumo039/start", "%lu", *start);
            motor_forward(255,0);
            while(dig.r3 == 1 || dig.r2 == 1 || dig.r1 == 1 || dig.l1 == 1 || dig.l2 == 1 || dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            lines++;
        }
    }
}
void sumo(TickType_t *start) // sumo ring + hit detection
{
    struct sensors_ dig; struct accData_ data;
    TickType_t hit; TickType_t end;
    while(SW1_Read() == 1) // runs till you press the button
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 || dig.r2 == 1 || dig.r1 == 1 || dig.l1 == 1 || dig.l2 == 1 || dig.l3 == 1) // if it touches the edgeline at all
        {
            edgeturn();
        }
        LSM303D_Read_Acc(&data);
        if(data.accX < -5000) // read hits
        {
            hit = xTaskGetTickCount();
            print_mqtt("Zumo039/hit", "%lu", hit);
            while(data.accX < 10000) // trying to prevent infinte hit messages
            {
                LSM303D_Read_Acc(&data);
                reflectance_digital(&dig);
                if(dig.r3 == 1 || dig.r2 == 1 || dig.r1 == 1 || dig.l1 == 1 || dig.l2 == 1 || dig.l3 == 1)
                {
                    edgeturn();
                }
            }
        }
    } // if pushed the button send stop & final time
    motor_forward(0,0);
    motor_stop();
    end = xTaskGetTickCount();
    print_mqtt("Zumo039/stop", "%lu", end);
    print_mqtt("Zumo039/time", "%lu", end - *start);
}
void zmain(void)
{
    TickType_t start; IR_Start(); IR_flush(); LSM303D_Start();
    reflectance_start(); // Ultra_Start();  distance = Ultra_GetDistance(); int distance; 
    reflectance_set_threshold(12063, 10140, 8261, 8266, 9491, 10675);    //reflectance_set_threshold(7300, 6400, 5800, 5800, 6000, 6600);    

    startup(&start);
    sumo(&start);
    while(true)
    {
        vTaskDelay(100); 
    }
}   
#endif

//Line Follower - DONE!
#if 0
//Line Follower
void startup() // basic startup
{
    motor_start();
    motor_forward(0,0);
    while(SW1_Read() == 1);
    vTaskDelay(1500);
    motor_forward(200,0);
} 
void first_line(TickType_t *start) // the first line
{
    struct sensors_ dig; int lines = 0;
    while(lines == 0)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            print_mqtt("Zumo039/ready", "line");
            IR_wait(); // vTaskDelay(1000);
            *start = xTaskGetTickCount();
            print_mqtt("Zumo039/start", "%lu", *start);
            motor_forward(255,0);            
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            lines++;
        }
    }
}
void course_finish(TickType_t *start) // the race course
{
    TickType_t miss; TickType_t end;
    struct sensors_ dig; int lines = 1; int missline = 1; 
    while(lines < 3) // keeps running till it hits the second line at finish
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            if(lines == 1) // 1st finish line 
            {
                while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
                {
                    reflectance_digital(&dig);
                }
                lines++;
            }
            else if(lines == 2) //2nd finish line (stops here)
            {
                motor_forward(0,0);
                motor_stop();
                end = xTaskGetTickCount();
                print_mqtt("Zumo039/stop", "%lu", end);
                print_mqtt("Zumo039/time", "%lu", end - *start);
                lines++;
            }
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(255,0);
            if(missline == 0) // check if center censors come back to line
            {
                miss = xTaskGetTickCount();
                print_mqtt("Zumo039/line", "%lu", miss);
                missline = 1;
            }        
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(25,225,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(255,25,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(45,255,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(255,45,0); 
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(0,240,0);
            if(missline == 1) // check if center censors miss the line
            {
                miss = xTaskGetTickCount();
                print_mqtt("Zumo039/miss", "%lu", miss);
                missline = 0;
            }
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(240,0,0);
            if(missline == 1) // check if center censors miss the line
            {
                miss = xTaskGetTickCount();
                print_mqtt("Zumo039/miss", "%lu", miss);
                missline = 0;
            }
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 1) // turn super tight left
        {
            motor_turn(0,155,0);
            if(missline == 1) // check if center censors miss the line
            {
                miss = xTaskGetTickCount();
                print_mqtt("Zumo039/miss", "%lu", miss);
                missline = 0;
            }  
        }
        else if(dig.r3 == 1 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn super tight right
        {
            motor_turn(155,0,0);
            if(missline == 1) // check if center censors miss the line
            {
                miss = xTaskGetTickCount();
                print_mqtt("Zumo039/miss", "%lu", miss);
                missline = 0;
            }
        }
    }
}   
void zmain(void)
{
    TickType_t start;
    IR_Start(); IR_flush(); // clear IR receive buffer
    reflectance_start(); reflectance_set_threshold(10063, 8140, 6261, 6266, 7491, 8675);
    
    startup(); 
    first_line(&start);
    course_finish(&start);
    while(true)
    {
        vTaskDelay(100);
    }
}   
#endif

//Maze - WIP! bugs or blind eyes ruin everything / FYI Fucntions use their variables because DESPERATION! It used to use only pointers but :sadface:.......
#if 0
//Maze
    /* FYI Fucntions use their variables because DESPERATION! It used to use only pointers but :sadface:.......
    100 speed tankturns                                                                             
        motor_tankright(100,1665); motor_tankright(100,1650); motor_tankright(100,1420); // 180 degree turn (right) V2              
        motor_tankleft(100,1640); motor_tankleft(100,1380); motor_tankleft(100,1230);// 180 degree turn (left) V2                 
        motor_tankright(100,810); motor_tankright(100,760); motor_tankright(100,630);   // 90 degree turn (right) V2                
        motor_tankleft(100,790); motor_tankleft(100,700); motor_tankleft(100,580); // 90 degree turn (left) V2                    
    turn preferences for each direction  
        forward: left(left), right(right), backward(down)
        left: right(up), backward(right), left(down)
        right: left(up), backward(left), right(down)
        backward: left(right), right(left), backward (up)
    */
void motor_tankright (uint8 speed,uint32 delay) // tankturn right
{
    MotorDirLeft_Write(0);      // set LeftMotor backward mode
    MotorDirRight_Write(1);     // set RightMotor forward mode
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    vTaskDelay(delay);
}
void motor_tankleft (uint8 speed,uint32 delay) // tankturn left
{
    MotorDirLeft_Write(1);      // set LeftMotor forward mode
    MotorDirRight_Write(0);     // set RightMotor backward mode
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    vTaskDelay(delay);
}
void right90() // 90 right turn
{
    motor_tankright(100,630);
}
void right180() // 180 right turn
{
    motor_tankright(100,1420);
}
void left90() // 90 left turn
{
    motor_tankleft(100,580);
}
void left180() // 180 left turn
{
    motor_tankleft(100,1230);
}
void startup(TickType_t *start) // startup + first line
{
    struct sensors_ dig; int lines = 0;
    motor_forward(0,0);
    while(SW1_Read() == 1);
    vTaskDelay(1500);
    motor_forward(100,0);
    while(lines == 0) // startup line
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            print_mqtt("Zumo039/ready", "maze");
            IR_wait(); //vTaskDelay(1000);
            *start = xTaskGetTickCount();
            print_mqtt("Zumo039/start", "%lu", *start);
            motor_forward(100,0);            
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            lines++;
        }
    }
}
void forwardmode(int *direction, int *x, int *y) // forward
{
    int distance;
    int temp = *y; int temp2 = *x; int lock =0;
    if(temp == 11)
    {
        if(temp2 < 0)
        {
            right90();
            *direction = 2;
            motor_forward(100, 130);
            return;
        }
        if(temp2 > 0)
        {
            left90();
            *direction = 1;
            motor_forward(100, 130);
            return;
        }
        motor_forward(100, 130);
        return;
    }
    distance = Ultra_GetDistance(); // check forward
    if(distance <= 10) 
    {
        left90(); // turn left 90 degrees (left mode?)
        motor_forward(0, 0);
        lock = 1;
    }
    else
    {
        motor_forward(100, 130);
        return;
    }
    distance = Ultra_GetDistance(); // check left
    if(lock == 1){
    if(distance <= 10)
    {
        right180(); // turn right 180 degrees (right mode?)
        motor_forward(0, 0);
        *direction = 2; lock = 0;
        motor_forward(100, 130);
        return;
    }
    else
    {
        *direction = 1;
        motor_forward(100, 130);
        return;
    }}
}
void forwardleftcorner(int *direction, int *y) // forward left corner
{
    int distance; int lock = 0;
    if(*y == 11)
    {
        right90();
        *direction = 2;
        motor_forward(100, 130);
        return;
    }
    distance = Ultra_GetDistance(); // check forward
    if(distance <= 10) 
    {
        right90(); //
        motor_forward(0, 0);
        lock = 1;
    }
    else
    {
        motor_forward(100, 130);
        return;
    }
    distance = Ultra_GetDistance(); // check left
    if(lock == 1){
    if(distance <= 10)
    {
        right90();
        motor_forward(0, 0);
        *direction = 3; // backward
        lock = 0;
        motor_forward(100, 130);
        return;
    }
    else
    {
        *direction = 1; //left
        motor_forward(100, 130);
        return;
    }}
}
void forwardrightcorner(int *direction, int *y) // forward right corner
{
    int distance; int lock = 0;
    if(*y == 11)
    {
        left90();
        *direction = 1; // left
        motor_forward(100, 130);
        return;
    }
    distance = Ultra_GetDistance(); // check forward
    if(distance <= 10) 
    {
        left90(); // turn left 90 degrees (left mode?)
        motor_forward(0, 0);
        *direction = 1; //left
        lock = 0;
        motor_forward(100, 130);
        return;
    }
    else
    {
        motor_forward(100, 130);
        return;
    }
}
void leftmode(int *direction, int *x, int *y)    // left mode
{
    int distance; int temp = *y; int temp2 = *x; int lock = 0;
    if(temp == 11)
    {
        if(temp2 < 0)
        {
            left180();
            *direction = 2;
            motor_forward(100, 130);
            return;
        }
        else if(temp2 > 0)
        {
            motor_forward(100, 130);
            return;
        }
        else
        {
            right90();
            *direction = 0;
            motor_forward(100, 130);
            return;
        }
    }
    if(temp2 == -3)
    {
        right90();
        motor_forward(0,0);
        distance = Ultra_GetDistance(); // check forward
        if(distance <= 10) 
        {
            right90(); // turn left 90 degrees (left mode?)
            motor_forward(0, 0);
            *direction = 2; // right
            motor_forward(100, 130);
            return;
        }
        else
        {
            *direction = 0; // forward
            motor_forward(100, 130);
            return;
        }
    }
    right90();
    motor_forward(0,0);
    distance = Ultra_GetDistance(); // check forward
    if(distance <= 10) 
    {
        left90(); // turn left 90 degrees (left mode?)
        motor_forward(0, 0);
        lock = 1;
    }
    else
    {
        *direction = 0; // forward
        motor_forward(100, 130);
        return;
    }
    distance = Ultra_GetDistance(); // check left
    if(lock == 1){
    if(distance <= 10)
    {
        right180(); // turn right 180 degrees (right mode?)
        motor_forward(0, 0);
        *direction = 2; // right
        lock = 0;
        motor_forward(100, 130);
        return;
    }
    else
    {
        *direction = 1; // left
        motor_forward(100, 130);
        return;
    }}
}
void leftcorner(int *direction)    // left left corner (right not possible)
{
    int distance;
    right90();
    motor_forward(0,0);
    distance = Ultra_GetDistance(); // check forward
    if(distance <= 10) 
    {
        right90(); // turn left 90 degrees (left mode?)
        motor_forward(0, 0);
         *direction = 2; // right
        motor_forward(100, 130);
        return;
    }
    else
    {
        *direction = 0; // forward
        motor_forward(100, 130);
        return;
    }
}
void rightmode(int *direction, int *x, int *y)   // right mode
{
    int distance; int temp = *y; int temp2 = *x; int lock = 0;
    if(temp == 11)
    {
        if(temp2 < 0)
        {
            right180();
            *direction = 2;
            motor_forward(100, 130);
            return;
        }
        else if(temp2 > 0)
        {
            motor_forward(100, 130);
            return;
        }
        else
        {
            left90();
            *direction = 0;
            motor_forward(100, 130);
            return;
        }
    }
    if(temp2 == 3)
    {
        left90();
        motor_forward(0,0);
        distance = Ultra_GetDistance(); // check forward
        if(distance <= 10) 
        {
            left90(); // turn left 90 degrees (left mode?)
            motor_forward(0, 0);
            *direction = 1; // left
            motor_forward(100, 130);
        }
        else
        {
            *direction = 0; // forward
            motor_forward(100, 130);
            return;
        }
    }
    left90();
    motor_forward(0,0);
    distance = Ultra_GetDistance(); // check forward
    if(distance <= 10) 
    {
        right90(); // turn left 90 degrees (left mode?)
        motor_forward(0, 0);
        lock = 1;
    }
    else
    {
        *direction = 0; // forward
        motor_forward(100, 130);
        return;
    }
    distance = Ultra_GetDistance(); // check left
    if(lock == 1){
    if(distance <= 10)
    {
        left180(); // turn right 180 degrees (right mode?)
        motor_forward(0, 0);
        *direction = 1; // left
        lock = 0;
        motor_forward(100, 130);
        return;
    }
    else
    {
        *direction = 2; // right
        motor_forward(100, 130);
        return;
    }}
}
void rightcorner(int *direction)    // right right corner (left not possible)
{
    int distance;
    left90();
    motor_forward(0,0);
    distance = Ultra_GetDistance(); // check forward
    if(distance <= 10) 
    {
        left90(); // turn left 90 degrees (left mode?)
        motor_forward(0, 0);
         *direction = 2; // right
        motor_forward(100, 130);
        return;
    }
    else
    {
        *direction = 0; // forward
        motor_forward(100, 130);
        return;
    }
}
void finalemode(int *x, int *y, TickType_t *start)
{
    struct sensors_ dig; TickType_t end; int maze;
    while(maze==0)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1) // straight line
        {
            *y = *y + 1;
            motor_forward(100,0);
            print_mqtt("Zumo039/position", "%d %d", *x, *y);
            while(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
            {
                reflectance_digital(&dig);
            }
            if (*y == 13)
            {
                maze = 1;
            }
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(50,100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(100,50,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(75,100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(100,75,0); 
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(25,100,0);
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(100,25,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 1) // turn super tight left
        {
            motor_turn(0,100,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn super tight right
        {
            motor_turn(100,0,0);
        }
    }
    while(maze == 1)
    {
        reflectance_digital(&dig);
        if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // white EXIT/FINALE line
        {
            motor_forward(0,0);
            end = xTaskGetTickCount();
            print_mqtt("Zumo039/stop", "%lu", end);
            print_mqtt("Zumo039/time", "%lu", end - *start);
            motor_stop();
            maze++;
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(50,100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(100,50,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(75,100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(100,75,0); 
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(25,100,0);
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(100,25,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 1) // turn super tight left
        {
            motor_turn(0,100,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn super tight right
        {
            motor_turn(100,0,0);
        }
    }
}
void zmain(void)
{
    motor_start(); Ultra_Start(); IR_Start(); IR_flush();
    TickType_t start; struct sensors_ dig;
    int x = 0, y = -1, direction = 0, maze = 0;; // directions: 0 = forward, 1 = left, 2 = right, 3 = backward
    reflectance_start(); reflectance_set_threshold(7300, 6400, 5800, 5800, 6100, 6600);  // coordinate limits: x: -3, y: 0 --- x: 3, y: 13
   
    startup(&start);
    while(maze == 0) // maze course
    {
        reflectance_digital(&dig);
        if (x==0 && y==13)
        {
            maze = 1;
            
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1) // straight line
        {
            if(direction == 0)
            {
                y++;
                motor_forward(100, 255);
                motor_forward(0, 0);
                print_mqtt("Zumo039/position", "%d %d", x, y);
                forwardmode(&direction, &x, &y);
            }
            else if(direction == 1)
            {
                x--;
                motor_forward(100, 255);
                motor_forward(0, 0);
                print_mqtt("Zumo039/position", "%d %d", x, y);
                leftmode(&direction, &x, &y);
            }
            else if(direction == 2)
            {
                x++;
                motor_forward(100, 255);
                motor_forward(0, 0);
                print_mqtt("Zumo039/position", "%d %d", x, y);
                rightmode(&direction, &x, &y);
            }
        }
        
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1) // left corner
        {
            if(direction == 0)
            {
                y++;
                motor_forward(100, 255);
                motor_forward(0, 0);
                print_mqtt("Zumo039/position", "%d %d", x, y);
                forwardleftcorner(&direction, &y);
            }
            else if(direction == 1)
            {
                x--;
                motor_forward(100, 255);
                motor_forward(0, 0);
                print_mqtt("Zumo039/position", "%d %d", x, y);
                leftcorner(&direction);
            }
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // right corner
        {
            if(direction == 0)
            {
                y++;
                motor_forward(100, 255);
                motor_forward(0, 0);
                print_mqtt("Zumo039/position", "%d %d", x, y);
                forwardrightcorner(&direction, &y);
            }
            else if(direction == 2)
            {
                x++;
                motor_forward(100, 255);
                motor_forward(0, 0);
                print_mqtt("Zumo039/position", "%d %d", x, y);
                rightcorner(&direction);
            }
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(50,100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(100,50,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(75,100,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(100,75,0); 
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(25,100,0);
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(100,25,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 1) // turn super tight left
        {
            motor_turn(0,100,0);
        }
        else if(dig.r3 == 1 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn super tight right
        {
            motor_turn(100,0,0);
        }
    }
    finalemode(&x, &y, &start);
    while(true)
    {
        vTaskDelay(100);
    }
}   
#endif

// Random commands + default good turning speeds
/*      
else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1) // left corner
        {
            *y = *y + 1;
            motor_forward(100, 255);
            motor_forward(0, 10);
            print_mqtt("Zumo039/position", "%d %d", *x, *y);
            if(*y == 11)
            {
                    right90();
                    *direction = 'r';
                    motor_forward(100, 100);
            }
            *distance = Ultra_GetDistance(); // check forward
            if(*distance <= 9) 
            {
                right90(); // turn right 90 degrees (right mode?)
                motor_forward(0, 10);
            }
            else
            {
                motor_forward(100, 100);
                return;
            }
            *distance = Ultra_GetDistance();  // check right
            if(*distance <= 9)
            {
                right90(); // turn right 90 degrees (backward-reverse mode?)
                *direction = 'b';
                motor_forward(100, 100);
                return;
            }
            else
            {
                *direction = 'r';
                motor_forward(100, 100);
                return;
            }     
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // right corner
        {
            *y = *y + 1;
            motor_forward(100, 255);
            motor_forward(0, 5);
            print_mqtt("Zumo039/position", "%d %d", *x, *y);
            if(*y == 11)
            {
                left90();
                *direction = 'l';
                motor_forward(100, 100);
                return;
            }
            *distance = Ultra_GetDistance(); // check forward
            if(*distance <= 9) 
            {
                left90(); // turn left 90 degrees (left mode?)
                motor_forward(0, 10);
            }
            else
            {
                motor_forward(100, 100);
                return;
            }
            *distance = Ultra_GetDistance();  // check left
            if(*distance <= 9)
            {
                left90(); // turn left 90 degrees (backward-reverse mode?)
                *direction = 'b';
                motor_forward(100, 100);
                return;
            }
            else
            {
                *direction = 'l';
                motor_forward(100, 100);
                return;
            }     
        }
        // struct sensors_ ref;
        // reflectance_read(&ref);
        // printf("%5d %5d %5d %5d %5d %5d\r\n", ref.l3, ref.l2, ref.l1,ref.r1, ref.r2, ref.r3);
        // struct sensors_ dig;
        // reflectance_digital(&dig);
        // printf("%5d %5d %5d %5d %5d %5d\r\n", dig.l3, dig.l2, dig.l1,dig.r1, dig.r2, dig.r3);
        // reflectance_set_threshold(5500, 4600, 4000, 4000, 4200, 4800); - kinda safe white thresholds

        200 speed tankturns
        motor_tankright(200,1000); // 360 degree turn (right)
        motor_tankleft(200,1000); // 360 degree turn (left)
        motor_tankright(200,750); // 270 degree turn (right)
        motor_tankleft(200,750); // 270 degree turn (left)

        motor_tankright(200,541); // 180 degree turn (right)
        motor_tankleft(200,530); // 180 degree turn (left)
        motor_tankright(200,462); // 150 degree turn (right)
        motor_tankleft(200,461); // 150 degree turn (left)
        motor_tankright(200,350); // 120 degree turn (right)
        motor_tankleft(200,340); // 120 degree turn (left)
        motor_tankright(200,265); // 90 degree turn (right)
        motor_tankleft(200,264); // 90 degree turn (left)

        100 speed tankturns
        motor_tankright(100,1665); // 180 degree turn (right)
        motor_tankleft(100,1640); // 180 degree turn (left)
        motor_tankright(100,810); // 90 degree turn (right)
        motor_tankleft(100,790); // 90 degree turn (left)

         if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1) // line detection
        {
            motor_forward(0,0);
        }
        
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(200,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(25,200,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(200,25,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(40,180,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(180,40,0); 
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(200,75,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(75,200,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 1) // turn super tight left
        {
            motor_turn(200,20,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn super tight right
        {
            motor_turn(20,200,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left 90 degrees
        {
            motor_turn(200,0,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right 90 degrees
        {
            motor_turn(0,200,0);
            
        }
        printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
        // vTaskDelay(100);

        V2 TURN SPEEDS
        if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 1)
        {
            motor_forward(0,0);
            motor_stop();
            lines++;
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // forward
        {
            motor_forward(200,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 1 && dig.l3 == 0) // turn left
        {
            motor_turn(35,200,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 1 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn right
        {
            motor_turn(200,35,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0) // turn baby left
        {
            motor_turn(95,200,0);
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 1 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn baby right
        {
            motor_turn(200,95,0); 
            
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 1 && dig.l3 == 1) // turn tight left
        {
            motor_turn(20,200,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 1 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn tight right
        {
            motor_turn(200,20,0);
        }
        else if(dig.r3 == 0 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 1) // turn super tight left
        {
            motor_turn(0,200,0);
            
        }
        else if(dig.r3 == 1 && dig.r2 == 0 && dig.r1 == 0 && dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0) // turn super tight right
        {
            motor_turn(200,0,0);
        }
[] END OF FILE*/