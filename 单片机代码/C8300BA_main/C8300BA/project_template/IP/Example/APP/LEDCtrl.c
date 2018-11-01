#include "ht32.h"
#include "ht32_board.h"
#include "mast.h"
#include "math.h"
#include "uart_protocal.h"


u32 CH3_Frequency;
u32 CH3_DeltaCCR;
_LED_CTRL LedCtrl;
    float Ration1,Ration2;
u16 x = 0,y = 0;

void GPTM0_PWMInit(void)
{
    
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
  TM_OutputInitTypeDef TM_OutputInitStructure;
  uint32_t wCRR = 0, wPSCR = 0;

  /* Enable PCLK of BUZZER and AFIO                                                                         */
  BUZZER_TM_CLK(CKCUClock) = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Configure the BUZZER_GPIO_PIN as TM channel output AFIO function                                       */
  HT32F_DVB_GPxConfig(BUZZER_GPIO_ID, BUZZER_AFIO_PIN, BUZZER_AFIO_MODE);

  /* Compute CRR and PSCR value                                                                             */
  wCRR = (48000000 / 25000) - 1;
  while ((wCRR / (wPSCR + 1)) > 0xFFFF)
  {
    wPSCR++;
  }
  wCRR = wCRR / (wPSCR + 1);
  

  AFIO_GPxConfig(GPIO_PB,AFIO_PIN_6,AFIO_FUN_MCTM_GPTM);
  /* Init BUZZER TM time-base                                                                               */
  TM_TimeBaseInitStructure.CounterReload = wCRR - 1;
  TM_TimeBaseInitStructure.Prescaler = wPSCR;
  TM_TimeBaseInitStructure.RepetitionCounter = 0;
  TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
  TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
  TM_TimeBaseInit(HT_GPTM0, &TM_TimeBaseInitStructure);

  /* Clear Update Event Interrupt flag                                                                      */
  TM_ClearFlag(HT_GPTM0, TM_FLAG_UEV);

  /* Init BUZZER TM Channel x to output PWM waveform with 33% duty                                          */
  TM_OutputInitStructure.Channel = TM_CH_3;
  TM_OutputInitStructure.OutputMode = TM_OM_PWM1;
  TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
  TM_OutputInitStructure.ControlN = TM_CHCTL_DISABLE;//TM_CHCTL_ENABLE;
  TM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
  TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
  TM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
  TM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
  TM_OutputInitStructure.Compare = ((wCRR + 1) * (100 - 33)) / 100;
  TM_OutputInit(HT_GPTM0, &TM_OutputInitStructure);
   
  /* Enable GPTM                                                                                            */
  TM_Cmd(HT_GPTM0, ENABLE);
    
}

void HT32_MCTM0_PWM_Init(void)
{
    TM_TimeBaseInitTypeDef MCTM_TimeBaseInitStructure;
    TM_OutputInitTypeDef MCTM_OutputInitStructure;
    MCTM_CHBRKCTRInitTypeDef MCTM_CHBRKCTRInitStructure;
    HT32F_DVB_GPxConfig(LED_PWM_GPIO_ID, LED_PWM_GPIO_PIN, LED_PWM_AFIO_MODE);
    /* MCTM Time Base configuration                                                                           */
    MCTM_TimeBaseInitStructure.CounterReload = HTCFG_MCTM_RELOAD - 1;
    MCTM_TimeBaseInitStructure.Prescaler = 0;  
    MCTM_TimeBaseInitStructure.RepetitionCounter = 0;
    MCTM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    MCTM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_MCTM0, &MCTM_TimeBaseInitStructure);

    MCTM_OutputInitStructure.Channel = LED_EN_TM_CHANNEL;
    MCTM_OutputInitStructure.OutputMode = TM_OM_PWM1;
    MCTM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
    MCTM_OutputInitStructure.ControlN = TM_CHCTL_DISABLE;
    MCTM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
    MCTM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
    MCTM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
    MCTM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
    MCTM_OutputInitStructure.Compare = 0;
    TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

    /* MCTM Off State, lock, Break, Automatic Output enable, dead time configuration                          */
    MCTM_CHBRKCTRInitStructure.OSSRState = MCTM_OSSR_STATE_ENABLE;
    MCTM_CHBRKCTRInitStructure.OSSIState = MCTM_OSSI_STATE_ENABLE;
    MCTM_CHBRKCTRInitStructure.LockLevel = MCTM_LOCK_LEVEL_2;
    MCTM_CHBRKCTRInitStructure.Break0 = MCTM_BREAK_DISABLE; //MCTM_BREAK_ENABLE;
    MCTM_CHBRKCTRInitStructure.Break0Polarity = MCTM_BREAK_POLARITY_LOW;
    MCTM_CHBRKCTRInitStructure.AutomaticOutput = MCTM_CHAOE_ENABLE;
    MCTM_CHBRKCTRInitStructure.DeadTime = HTCFG_MCTM_DEAD_TIME;
    MCTM_CHBRKCTRInitStructure.BreakFilter = 0;
    MCTM_CHBRKCTRConfig(HT_MCTM0, &MCTM_CHBRKCTRInitStructure);                                                                      
    TM_Cmd(HT_MCTM0, ENABLE);
    MCTM_CHMOECmd(HT_MCTM0, ENABLE);
//    TM_IntConfig(HT_MCTM0, TM_INT_UEV, ENABLE);  //TM_INT_CH0CC
//    NVIC_EnableIRQ(MCTM0_IRQn);
}

void GPIO_Init(void)
{  
    //LED提示
    HT32F_DVB_GPxConfig(LED_RED_GPIO_ID, LED_RED_GPIO_PIN, AFIO_FUN_GPIO);
    HT32F_DVB_GPxConfig(LED_GREEN_GPIO_ID, LED_GREEN_GPIO_PIN, AFIO_FUN_GPIO);
    
    GPIO_PullResistorConfig(LED_RED_GPIO, LED_RED_GPIO_PIN, GPIO_PR_DISABLE);
    GPIO_PullResistorConfig(LED_GREEN_GPIO, LED_GREEN_GPIO_PIN, GPIO_PR_DISABLE);
    
    GPIO_DriveConfig(LED_RED_GPIO, LED_RED_GPIO_PIN, GPIO_DV_4MA);
    GPIO_DriveConfig(LED_GREEN_GPIO, LED_GREEN_GPIO_PIN, GPIO_DV_4MA);
    
    GPIO_DirectionConfig(LED_RED_GPIO, LED_RED_GPIO_PIN, GPIO_DIR_OUT);
    GPIO_DirectionConfig(LED_GREEN_GPIO, LED_GREEN_GPIO_PIN, GPIO_DIR_OUT);
  
    
    
    //LED1 2 3 4使能
    HT32F_DVB_GPxConfig(LED_EN1_GPIO_ID, LED_EN1_GPIO_PIN, AFIO_FUN_GPIO);
    HT32F_DVB_GPxConfig(LED_EN2_GPIO_ID, LED_EN2_GPIO_PIN, AFIO_FUN_GPIO);
    HT32F_DVB_GPxConfig(LED_EN3_GPIO_ID, LED_EN3_GPIO_PIN, AFIO_FUN_GPIO);
    HT32F_DVB_GPxConfig(LED_EN4_GPIO_ID, LED_EN4_GPIO_PIN, AFIO_FUN_GPIO);
    
    GPIO_PullResistorConfig(LED_EN1_GPIO, LED_EN1_GPIO_PIN, GPIO_PR_DISABLE);
    GPIO_PullResistorConfig(LED_EN2_GPIO, LED_EN2_GPIO_PIN, GPIO_PR_DISABLE);
    GPIO_PullResistorConfig(LED_EN3_GPIO, LED_EN3_GPIO_PIN, GPIO_PR_DISABLE);
    GPIO_PullResistorConfig(LED_EN4_GPIO, LED_EN4_GPIO_PIN, GPIO_PR_DISABLE);
    
    GPIO_DriveConfig(LED_EN1_GPIO, LED_EN1_GPIO_PIN, GPIO_DV_4MA);
    GPIO_DriveConfig(LED_EN2_GPIO, LED_EN2_GPIO_PIN, GPIO_DV_4MA);
    GPIO_DriveConfig(LED_EN3_GPIO, LED_EN3_GPIO_PIN, GPIO_DV_4MA);
    GPIO_DriveConfig(LED_EN4_GPIO, LED_EN4_GPIO_PIN, GPIO_DV_4MA);
    
    GPIO_DirectionConfig(LED_EN1_GPIO, LED_EN1_GPIO_PIN, GPIO_DIR_OUT);
    GPIO_DirectionConfig(LED_EN2_GPIO, LED_EN2_GPIO_PIN, GPIO_DIR_OUT);
    GPIO_DirectionConfig(LED_EN3_GPIO, LED_EN3_GPIO_PIN, GPIO_DIR_OUT);
    GPIO_DirectionConfig(LED_EN4_GPIO, LED_EN4_GPIO_PIN, GPIO_DIR_OUT);

    //按键输入
    HT32F_DVB_GPxConfig(SW1_GPIO_ID, SW1_GPIO_PIN, SW1_AFIO_MODE);
    GPIO_PullResistorConfig(SW1_GPIO, SW1_GPIO_PIN, GPIO_PR_DISABLE);
    GPIO_DriveConfig(SW1_GPIO, SW1_GPIO_PIN, GPIO_DV_4MA);
    GPIO_DirectionConfig(SW1_GPIO, SW1_GPIO_PIN, GPIO_DIR_IN); 
    GPIO_InputConfig(SW1_GPIO, SW1_GPIO_PIN, ENABLE); 
    //充电状态输入
    HT32F_DVB_GPxConfig(BAT_STA_GPIO_ID, BAT_STA_GPIO_PIN, BAT_STA_AFIO_MODE);
    GPIO_PullResistorConfig(BAT_STA_GPIO, BAT_STA_GPIO_PIN, GPIO_PR_UP);
    GPIO_DirectionConfig(BAT_STA_GPIO, BAT_STA_GPIO_PIN, GPIO_DIR_IN); 
    GPIO_InputConfig(BAT_STA_GPIO, BAT_STA_GPIO_PIN, ENABLE); 
    
    //WIFI使能
    HT32F_DVB_GPxConfig(WIFI_EN_GPIO_ID, WIFI_EN_GPIO_PIN, WIFI_EN_AFIO_MODE);
    GPIO_PullResistorConfig(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, GPIO_PR_UP);
    GPIO_DriveConfig(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, GPIO_DV_4MA);
    GPIO_DirectionConfig(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, GPIO_DIR_OUT); 

    LED_EN1_OFF;LED_EN2_OFF;LED_EN3_OFF;LED_EN4_OFF;
    LED_RED_OFF;
    LED_GREEN_OFF;
    
    LedCtrl.CoolDuty = 0;
    LedCtrl.Brightness = 0;
}


#define BRIGHTNESS_DEFAULT_DUTY 50  //默认占空比值，初始
#define COOL_WARM_DUTY          20  //默认占空比值，初始冷光比例

void CaluWarmCoolBrt()
{
    LedCtrl.BrtAllValu = (u16)LedCtrl.Brightness * 19+20; 
    LedCtrl.CoolLoadValu = (LedCtrl.BrtAllValu * LedCtrl.CoolDuty)/100;
    LedCtrl.WarmLoadValu = (LedCtrl.BrtAllValu * (100 - LedCtrl.CoolDuty))/100;
//    Ration1 = (COOL_WARM_DUTY) / 100;
 /*   
    if(LedCtrl.CoolDuty < 100)
    {
        Ration1 = 9;
        Ration2 = 1;
    }
    else if(LedCtrl.CoolDuty < 90)
    {
        Ration1 = 8;
        Ration2 = 2;
        
    }
    else if(LedCtrl.CoolDuty < 80)
    {
        Ration1 = 7;
        Ration2 = 3;
        
    }
    else if(LedCtrl.CoolDuty < 70)
    {
        Ration1 = 6;
        Ration2 = 4;
        
    }
    else if(LedCtrl.CoolDuty < 60)
    {
        Ration1 = 5;
        Ration2 =5;
        
    }
    else if(LedCtrl.CoolDuty < 50)
    {
        Ration1 = 4;
        Ration2 =6;
        
    }
     else if(LedCtrl.CoolDuty < 40)
    {
        Ration1 = 3;
        Ration2 =7;
        
    }
    else if(LedCtrl.CoolDuty < 30)
    {
        Ration1 = 2;
        Ration2 =8;
        
    }
    else if(LedCtrl.CoolDuty < 20)
    {
         Ration1 = 1;
        Ration2 = 9;     
    }  */
//    Ration2 = LedCtrl.CoolDuty;
//    Ration2 = Ration2 / 100;
//    Ration1 = 100-LedCtrl.CoolDuty;
//    Ration1 = Ration1 / 100;

//    if(Ration1 > Ration2)
//    {
//        Ration1 = (Ration1/Ration2);
//        Ration2 = 1;
//    }
//    else
//    {
//        Ration2 = (Ration2/Ration1);
//        Ration1 = 1;      
//    }

}
    float Temp = 0;

void LightOnOrOffSlowly(void)
{
    u16 Temp1;
    if(LedCtrl.OnOrOff == 1)            //仅开机后才改变
    {          
        if(LedCtrl.WarmLoadValu > LedCtrl.WarmLastLoadValu)
        {
//            x++;
//            Temp = Ration1 * x;
//            Temp1 = (u16)Temp;
//            LedCtrl.WarmLastLoadValu = Temp1;
            LedCtrl.WarmLastLoadValu++;
            
        }
        if(LedCtrl.WarmLoadValu < LedCtrl.WarmLastLoadValu)
        {
            LedCtrl.WarmLastLoadValu--;  
//            x--; 
        }
        if(LedCtrl.CoolLoadValu > LedCtrl.CoolLastLoadValu)
        {
//            y++;
//            Temp = Ration2 * y;            
//            Temp1 = (u16)Temp;
//            LedCtrl.CoolLastLoadValu = Temp1;
            LedCtrl.CoolLastLoadValu++;

        }
        if(LedCtrl.CoolLoadValu < LedCtrl.CoolLastLoadValu)
        {
            LedCtrl.CoolLastLoadValu--;
//            y--;
            
        }
        SetWarmBrt(LedCtrl.WarmLastLoadValu);
        SetCoolBrt(LedCtrl.CoolLastLoadValu);
    }
    else
    {
        if(LedCtrl.WarmLastLoadValu > 0)
        {
//            x--;
//            Temp = Ration1 * x;
//            Temp1 = (u16)Temp;
//            LedCtrl.WarmLastLoadValu = Temp1;
            LedCtrl.WarmLastLoadValu--;            
        }
        if(LedCtrl.CoolLastLoadValu > 0)
        {
//            y--;
//            Temp = Ration2 * y;            
//            Temp1 = (u16)Temp;
//            LedCtrl.CoolLastLoadValu = Temp1;
            LedCtrl.CoolLastLoadValu--;
        }
        SetWarmBrt(LedCtrl.WarmLastLoadValu);
        SetCoolBrt(LedCtrl.CoolLastLoadValu);
        if((LedCtrl.WarmLastLoadValu == 0)&&(LedCtrl.CoolLastLoadValu == 0))
        {
            LED_EN1_OFF;LED_EN2_OFF;LED_EN3_OFF;LED_EN4_OFF;
        }      
    }
}

void TurnOnOff_Light(bool  on)
{
    LedCtrl.OnOrOff = on;
	if(on)
	{
        if(LedCtrl.CoolDuty == 0) //如果没有设置变度，赋初值
        {
            LedCtrl.CoolDuty = COOL_WARM_DUTY;
        }
        if(LedCtrl.Brightness == 0) //如果没有设置变度，赋初值
        {
            LedCtrl.Brightness = BRIGHTNESS_DEFAULT_DUTY;
            CaluWarmCoolBrt();
        }
        x = 0;y=0;
        LED_EN1_ON;LED_EN2_ON;LED_EN3_ON;LED_EN4_ON;

	}
//	else
//	{
//	}
}

#define MAX_DUTY   1728     //最大占空比，超出灯会闪


void SetWarmBrt(u16 LoadValu)
{   
    HT_GPTM0->CH3CCR = LoadValu;  
}

void SetCoolBrt(u16 LoadValu)
{
    HT_MCTM0->CH1CCR = LoadValu;   
}

void CoolOn(void)
{
    
    CaluWarmCoolBrt();
}

void WarmOn(void)
{
    SetCoolBrt(LedCtrl.Brightness);
}

void SetBrightness(u8 Brt)
{
    if(LedCtrl.OnOrOff == 1)
    {
        LedCtrl.Brightness = Brt;
        CaluWarmCoolBrt();
//		SetWarmBrt(LedCtrl.WarmLoadValu);
//		SetCoolBrt(LedCtrl.CoolLoadValu);              
        
    }        
}


/*********************************************************************************************************
**函数名称:SetColor
**功能描述:设置颜色
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void SetColor(u8 Color)
{

}


/*********************************************************************************************************
**函数名称:SetColorWarm
**功能描述:设置冷色暖色(0-100) 暖光 101- 200暖色
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void SetColorWarm(u8 ColorWarm)
{
    if(LedCtrl.OnOrOff == 1)            //仅开机后才改变
    {
        LedCtrl.CoolDuty = ColorWarm;
        CaluWarmCoolBrt();
//		SetWarmBrt(LedCtrl.WarmLoadValu);
//		SetCoolBrt(LedCtrl.CoolLoadValu);              
        
    }
}


