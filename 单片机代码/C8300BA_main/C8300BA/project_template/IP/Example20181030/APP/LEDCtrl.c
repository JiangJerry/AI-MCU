#include "ht32.h"
#include "ht32_board.h"
#include "mast.h"
#include "uart_protocal.h"


u32 CH3_Frequency;
u32 CH3_DeltaCCR;
_LED_CTRL LedCtrl;


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
    
    //WIFI使能
    HT32F_DVB_GPxConfig(WIFI_EN_GPIO_ID, WIFI_EN_GPIO_PIN, WIFI_EN_AFIO_MODE);
    GPIO_PullResistorConfig(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, GPIO_PR_UP);
    GPIO_DriveConfig(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, GPIO_DV_4MA);
    GPIO_DirectionConfig(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, GPIO_DIR_OUT); 
    GPIO_WriteOutBits(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, SET);	   

    LED_EN1_OFF;LED_EN2_OFF;LED_EN3_OFF;LED_EN4_OFF;
    LED_RED_OFF;
    LED_GREEN_OFF;
}


#define BRIGHTNESS_DEFAULT_DUTY 2  //默认占空比值，初始 

void TurnOnOff_Light(bool  on)
{
    LedCtrl.OnOrOff = on;
	if(on)
	{
        if(LedCtrl.Brightness == 0) //如果没有设置变度，赋初值
        {
            LedCtrl.Brightness = BRIGHTNESS_DEFAULT_DUTY;
        }
        if(LedCtrl.CoolOrWarm > 100)                    //大于100冷色
        {
            SetColorWarm(LedCtrl.Brightness + 100);
        }
        else
        {
            SetColorWarm(LedCtrl.Brightness);           
        }        
	}
	else
	{
        LED_EN1_OFF;LED_EN2_OFF;LED_EN3_OFF;LED_EN4_OFF;

		if(LedCtrl.WarmLoadValu !=0)
			LedCtrl.WarmLastLoadValu = LedCtrl.WarmLoadValu;
        if(LedCtrl.CoolLoadValu != 0)
  			LedCtrl.CoolLastLoadValu = LedCtrl.CoolLoadValu;
         
        LedCtrl.WarmLoadValu = 0;
        LedCtrl.CoolLoadValu = 0;
		SetWarmBrt(0);
		SetCoolBrt(0);        
	}
}

#define MAX_DUTY   1728     //最大占空比，超出灯会闪


void SetWarmBrt(u8 Brt)
{   
    if(Brt > 0)
    {
        LedCtrl.WarmLoadValu = (u16)Brt * 19+20;     
        if(LedCtrl.WarmLoadValu > MAX_DUTY)
            LedCtrl.WarmLoadValu = MAX_DUTY;
    }
    else
    {
        LedCtrl.CoolLoadValu = 0;
    }
    HT_GPTM0->CH3CCR = LedCtrl.WarmLoadValu;  
}

void SetCoolBrt(u8 Brt)
{
    if(Brt > 0)
    {   
        LedCtrl.CoolLoadValu = (u16)Brt * 19+20;
        if(LedCtrl.CoolLoadValu > MAX_DUTY)
            LedCtrl.CoolLoadValu = MAX_DUTY; 
    }
    else
    {
        LedCtrl.WarmLoadValu = 0;        
    }
    HT_MCTM0->CH1CCR = LedCtrl.CoolLoadValu;   
}

void CoolOn(void)
{
    SetCoolBrt(LedCtrl.Brightness);
    SetWarmBrt(0);
    LED_EN1_OFF;LED_EN2_OFF;LED_EN3_ON;LED_EN4_ON;
}

void WarmOn(void)
{
    SetCoolBrt(0);
    SetWarmBrt(LedCtrl.Brightness);
    LED_EN1_ON;LED_EN2_ON;LED_EN3_OFF;LED_EN4_OFF;
}

void SetBrightness(u8 Brt)
{
    if(LedCtrl.OnOrOff == 1)
    {
        LedCtrl.Brightness = Brt;
        if(LedCtrl.CoolOrWarm > 100)    //大于100冷色
        {
            CoolOn();
        }
        else
        {
            WarmOn();          
        }
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
        LedCtrl.CoolOrWarm = ColorWarm;
        if(LedCtrl.CoolOrWarm > 100)    //大于100冷色
        {
            CoolOn();
        }
        else
        {
            WarmOn();
        }
    }
}


