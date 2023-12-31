#define COMP_STM32_INVERTINGINPUT_1_4VREFINT       (0x00000000U)
#define COMP_STM32_INVERTINGINPUT_1_2VREFINT       (0x1UL << (4U))
#define COMP_STM32_INVERTINGINPUT_3_4VREFINT       (0x2UL << (4U))
#define COMP_STM32_INVERTINGINPUT_VREFINT          ((0x2UL << (4U)) | (0x1UL << (4U)))
#define COMP_STM32_INVERTINGINPUT_DAC1             (0x4UL << (4U))
#define COMP_STM32_INVERTINGINPUT_DAC1SWITCHCLOSED ((0x4UL << (4U)) | (0x1UL << (1U)))
#define COMP_STM32_INVERTINGINPUT_DAC2             ((0x4UL << (4U)) | (0x1UL << (4U)))
#define COMP_STM32_INVERTINGINPUT_IO1              ((0x4UL << (4U)) | (0x2UL << (4U)))

#define COMP_STM32_NONINVERTINGINPUT_IO1              (0x00000000U)
#define COMP_STM32_NONINVERTINGINPUT_DAC1SWITCHCLOSED (0x1UL << (1U))

#define COMP_STM32_OUTPUT_NONE         (0x00000000U)
#define COMP_STM32_OUTPUT_TIM1BKIN     (0x1UL << (8U))
#define COMP_STM32_OUTPUT_TIM1IC1      (0x2UL << (8U))
#define COMP_STM32_OUTPUT_TIM1OCREFCLR ((0x2UL << (8U)) | (0x1UL << (8U)))
#define COMP_STM32_OUTPUT_TIM2IC4      (0x4UL << (8U))
#define COMP_STM32_OUTPUT_TIM2OCREFCLR ((0x4UL << (8U)) | (0x1UL << (8U)))
#define COMP_STM32_OUTPUT_TIM3IC1      ((0x4UL << (8U)) | (0x2UL << (8U)))
#define COMP_STM32_OUTPUT_TIM3OCREFCLR (0x7UL << (8U))
