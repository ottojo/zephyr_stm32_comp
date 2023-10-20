#define DT_DRV_COMPAT st_stm32f0_comparator

#include <stdbool.h>

#include <errno.h>
#include <stm32f0xx_hal.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

LOG_LEVEL_SET(LOG_LEVEL_INF);

struct stm32_comp_cfg {
  LOG_INSTANCE_PTR_DECLARE(log);
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

  HAL_StatusTypeDef res = HAL_COMP_Init(&hcomp);

  if (res != HAL_OK) {
    LOG_INST_ERR(cfg->log, "HAL_COMP_Init returned error 0x%x", (int)res);
    return -EIO;
  }

  res = HAL_COMP_Start(&hcomp);
  if (res != HAL_OK) {
    LOG_INST_ERR(cfg->log, "HAL_COMP_Start returned error 0x%x", (int)res);
    return -EIO;
  }

  int err = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);

  if (err != 0) {
    LOG_INST_ERR(cfg->log, "pinctrl_apply_state returned error %d", err);
    return err;
  }
  LOG_INST_INF(cfg->log, "Init done!");

  return 0;
}

#define COMP_STM32_INIT(inst)                                                  \
  LOG_INSTANCE_REGISTER(stm32_comp, inst, LOG_LEVEL_INF);                      \
  PINCTRL_DT_INST_DEFINE(inst);                                                \
  BUILD_ASSERT(IS_COMP_ALL_INSTANCE((COMP_TypeDef *)DT_INST_REG_ADDR(inst)),   \
               "Invalid base address " STRINGIFY(                              \
                   DT_INST_REG_ADDR(inst)) " for comparator instance!");       \
  static const struct stm32_comp_cfg stm32_comp_cfg_##inst = {                 \
      LOG_INSTANCE_PTR_INIT(log, stm32_comp, inst).pcfg =                      \
          PINCTRL_DT_INST_DEV_CONFIG_GET(inst),                                \
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
