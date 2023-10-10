
#define DT_DRV_COMPAT st_stm32_comparators

#include <stdbool.h>

#include <stm32f0xx_hal.h>
#include <sys/errno.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pinctrl.h>

struct stm32_comp_channel_cfg {
  int input_positive;
  int input_negative;
  bool invert_output;
  int output;
};
struct stm32_comp_cfg {
  const struct stm32_comp_channel_cfg **channels;
  const struct pinctrl_dev_config *pcfg;
};

struct stm32_comp_data {};

int stm32_comp_init(const struct device *dev) {

  const struct stm32_comp_cfg *cfg = dev->config;

  for (int i = 0; i < 2; i++) {
    const struct stm32_comp_channel_cfg *channel_cfg = cfg->channels[i];
    if (channel_cfg == NULL) {
      continue;
    }

    COMP_HandleTypeDef hcomp;

    // TODO: support more instances, probably by storing the instance in channel
    //  cfg once i figure out the required macrobatics to do that
    hcomp.Instance = i == 0 ? COMP1 : COMP2;
    hcomp.Init.InvertingInput = channel_cfg->input_negative;
    hcomp.Init.NonInvertingInput = channel_cfg->input_positive;
    hcomp.Init.Output = channel_cfg->output;
    hcomp.Init.OutputPol = channel_cfg->invert_output
                               ? COMP_OUTPUTPOL_INVERTED
                               : COMP_OUTPUTPOL_NONINVERTED;
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
  }

  int err = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
  if (err != 0) {
    return err;
  }

  return 0;
}

#define CAT2(a, b) DT_CAT(a, b)

#define CHANNEL_CONFIG_PTR_ARRAY_INITIALIZER(channel_node_ident, parent_inst)  \
  [DT_REG_ADDR(channel_node_ident) - 1] = &CAT2(                               \
      stm_comp_##parent_inst##_channel_, DT_REG_ADDR(channel_node_ident)),

#define CHANNELS_ARRAY(inst)                                                   \
  static const struct stm32_comp_channel_cfg *enabled_channels_##inst[2] = {   \
      DT_INST_FOREACH_CHILD_STATUS_OKAY_VARGS(                                 \
          inst, CHANNEL_CONFIG_PTR_ARRAY_INITIALIZER, inst)};

#define COMP_STM32_INIT_CHANNEL(channel_node_ident, parent_inst)               \
  static const struct stm32_comp_channel_cfg CAT2(                             \
      stm_comp_##parent_inst##_channel_, DT_REG_ADDR(channel_node_ident)) = {  \
      .input_positive = DT_PROP(channel_node_ident, zephyr_input_positive),    \
      .input_negative = DT_PROP(channel_node_ident, zephyr_input_negative),    \
      .invert_output = DT_PROP(channel_node_ident, zephyr_invert_output),      \
      .output = DT_PROP(channel_node_ident, zephyr_output),                    \
  };

#define COMP_STM32_INIT(inst)                                                  \
  PINCTRL_DT_INST_DEFINE(inst);                                                \
  DT_INST_FOREACH_CHILD_STATUS_OKAY_VARGS(inst, COMP_STM32_INIT_CHANNEL, inst) \
  CHANNELS_ARRAY(inst)                                                         \
  static const struct stm32_comp_cfg stm32_comp_cfg_##inst = {                 \
      .channels = enabled_channels_##inst,                                     \
      .pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),                            \
                                                                               \
  };                                                                           \
  static struct stm32_comp_data stm32_comp_data_##inst = {};                   \
  DEVICE_DT_INST_DEFINE(inst, stm32_comp_init, NULL, &stm32_comp_data_##inst,  \
                        &stm32_comp_cfg_##inst, POST_KERNEL, 80, NULL);

DT_INST_FOREACH_STATUS_OKAY(COMP_STM32_INIT)
