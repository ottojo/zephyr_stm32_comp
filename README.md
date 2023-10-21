```c
#include <zephyr/dt-bindings/comparator/stm32.h>

/ {
    soc {
        comp1: comp@4001001c {
            compatible = "st,stm32f0-comparator";
            status = "okay";
            reg = <0x4001001c 2>;

            pinctrl-0 = <&comp1_inp_pa1 &comp1_out_pa6>;
            pinctrl-names = "default";

            zephyr,input-noninverting = <COMP_STM32_NONINVERTINGINPUT_IO1>;
            zephyr,input-inverting = <COMP_STM32_INVERTINGINPUT_1_2VREFINT>;
            zephyr,invert-output;
            zephyr,output = <COMP_STM32_OUTPUT_NONE>;
        };

        comp2: comp@4001001e {
            compatible = "st,stm32f0-comparator";
            status = "okay";
            reg = <0x4001001e 2>;

            pinctrl-0 = <&comp2_inp_pa3 &comp2_out_pa2>;
            pinctrl-names = "default";

            zephyr,input-noninverting = <COMP_STM32_NONINVERTINGINPUT_IO1>;
            zephyr,input-inverting = <COMP_STM32_INVERTINGINPUT_1_2VREFINT>;
            zephyr,output = <COMP_STM32_OUTPUT_NONE>;
        };
    };
};
```
