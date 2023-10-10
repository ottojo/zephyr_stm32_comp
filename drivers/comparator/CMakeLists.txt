zephyr_library()

zephyr_library_sources_ifdef(CONFIG_COMPARATOR_STM32 comp_stm32.c)
