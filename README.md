```dts
#include <zephyr/dt-bindings/comparator/stm32.h>

/ {
    soc {
        comp@4001001c {
            compatible = "st,stm32-comparators";
            status = "okay";
            reg = <0x4001001C 1>;

            #address-cells = <1>;
            #size-cells = <0>;

            pinctrl-0 = <&comp1_inp_pa1 &comp1_out_pa6 &comp2_inp_pa3 &comp2_out_pa2>;
            pinctrl-names = "default";

            channel@1 {
                status = "okay";
                reg = <1>;
                zephyr,input-positive = <COMP_NONINVERTINGINPUT_IO1>;
                zephyr,input-negative = <COMP_INVERTINGINPUT_1_2VREFINT>;
                /delete-property/ zephyr,invert-output;
                zephyr,output = <COMP_OUTPUT_NONE>;
            };

            channel@2 {
                status = "okay";
                reg = <2>;
                zephyr,input-positive = <COMP_NONINVERTINGINPUT_IO1>;
                zephyr,input-negative = <COMP_INVERTINGINPUT_1_2VREFINT>;
                zephyr,invert-output;
                zephyr,output = <COMP_OUTPUT_NONE>;
            };
        };
    };
};
```
