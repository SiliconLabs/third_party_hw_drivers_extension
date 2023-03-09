#ifndef SCL_A172MRQ_CONFIG_H_
#define SCL_A172MRQ_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=true> A172MRQ_RESET
// $[GPIO_A172MRQ_RESET]
#define A172MRQ_RESET_PORT       gpioPortC
#define A172MRQ_RESET_PIN        8
// [GPIO_A172MRQ_RESET]$

// <gpio optional=true> A172MRQ_LD1
// $[GPIO_A172MRQ_LD1]
#define A172MRQ_LD1_PORT         gpioPortB
#define A172MRQ_LD1_PIN          0
// [GPIO_A172MRQ_LD1]$

// <gpio optional=true> A172MRQ_LD2
// $[GPIO_A172MRQ_LD2]
#define A172MRQ_LD2_PORT         gpioPortB
#define A172MRQ_LD2_PIN          1
// [GPIO_A172MRQ_LD2]$

// <gpio optional=true> A172MRQ_GP1
// $[GPIO_A172MRQ_GP1]
#define A172MRQ_GP1_PORT         gpioPortC
#define A172MRQ_GP1_PIN          0
// [GPIO_A172MRQ_GP1]$

// <gpio optional=true> A172MRQ_GP2
// $[GPIO_A172MRQ_GP2]
#define A172MRQ_GP2_PORT         gpioPortA
#define A172MRQ_GP2_PIN          0
// [GPIO_A172MRQ_GP2]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* SCL_A172MRQ_CONFIG_H_ */
