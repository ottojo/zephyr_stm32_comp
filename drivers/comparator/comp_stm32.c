#define DT_DRV_COMPAT st_stm32f0_comparator

#include <stdbool.h>

#include <stm32f0xx_hal.h>
#include <sys/errno.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pinctrl.h>

struct stm32_comp_cfg {
  const struct pinctrl_dev_config *pcfg;
  COMP_TypeDef *instance;
  int input_positive;
  int input_negative;
  bool invert_output;
  int output;
};

struct stm32_comp_data {};

int stm32_comp_init(const struct device *dev) {
  const struct stm32_comp_cfg *cfg = dev->config;

  COMP_HandleTypeDef hcomp;

  hcomp.Instance = cfg->instance;
  hcomp.Init.InvertingInput = cfg->input_negative;
  hcomp.Init.NonInvertingInput = cfg->input_positive;
  hcomp.Init.Output = cfg->output;
  hcomp.Init.OutputPol =
      cfg->invert_output ? COMP_OUTPUTPOL_INVERTED : COMP_OUTPUTPOL_NONINVERTED;
  hcomp.Init.Hysteresis = COMP_HYSTERESIS_NONE;
  hcomp.Init.Mode = COMP_MODE_HIGHSPEED;
  hcomp.Init.WindowMode = COMP_WINDOWMODE_DISABLE;
  hcomp.Init.TriggerMode = COMP_TRIGGERMODE_NONE;
  if (HAL_COMP_Init(&hcomp) != HAL_OK) {
    return -EIO;
  }

  if (HAL_COMP_Start(&hcomp) != HAL_OK) {
    return -EIO;
  }

  int err = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
  if (err != 0) {
    return err;
  }

  return 0;
}

#define COMP_STM32_INIT(inst)                                                  \
  PINCTRL_DT_INST_DEFINE(inst);                                                \
  BUILD_ASSERT(IS_COMP_ALL_INSTANCE((COMP_TypeDef *)DT_INST_REG_ADDR(inst)),   \
               "Invalid base address " STRINGIFY(                              \
                   DT_INST_REG_ADDR(inst)) " for comparator instance!");       \
  static const struct stm32_comp_cfg stm32_comp_cfg_##inst = {                 \
      .pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),                            \
      .instance = (COMP_TypeDef *)DT_INST_REG_ADDR(inst),                      \
      .input_positive = DT_INST_PROP(inst, zephyr_input_positive),             \
      .input_negative = DT_INST_PROP(inst, zephyr_input_negative),             \
      .invert_output = DT_INST_PROP(inst, zephyr_invert_output),               \
      .output = DT_INST_PROP(inst, zephyr_output),                             \
  };                                                                           \
  static struct stm32_comp_data stm32_comp_data_##inst = {};                   \
  DEVICE_DT_INST_DEFINE(inst, stm32_comp_init, NULL, &stm32_comp_data_##inst,  \
                        &stm32_comp_cfg_##inst, POST_KERNEL, 80, NULL);

DT_INST_FOREACH_STATUS_OKAY(COMP_STM32_INIT)
