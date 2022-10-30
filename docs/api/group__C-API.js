var group__C_API =
[
    [ "dh_filter_parameters", "structdh__filter__parameters.html", [
      [ "cutoff_frequency_high", "structdh__filter__parameters.html#af48493ea08332cbd08cfcd0f3f0488c3", null ],
      [ "cutoff_frequency_low", "structdh__filter__parameters.html#a9666f8a187c22af6d7881e68d4dd5f80", null ],
      [ "filter_order", "structdh__filter__parameters.html#a47fc847d64ae46cb15163232f853fe89", null ],
      [ "filter_type", "structdh__filter__parameters.html#a060e3855b3be2091e8424af22e26094c", null ],
      [ "ripple", "structdh__filter__parameters.html#ad8301fe5af5acd90b2137c1b5bcee659", null ],
      [ "sampling_frequency", "structdh__filter__parameters.html#a44fc36ad5305d7c7b59b3483fe029637", null ]
    ] ],
    [ "dh_filter_data", "structdh__filter__data.html", [
      [ "buffer", "structdh__filter__data.html#a41e070ebd42659262703bcddd7157f67", null ],
      [ "buffer_length", "structdh__filter__data.html#a1a61faf462b14ff16e6481e89e5affe7", null ],
      [ "buffer_needs_cleanup", "structdh__filter__data.html#ab684a085d4a1a1db0774b9844796b629", null ],
      [ "coefficients_in", "structdh__filter__data.html#afb77232659509847e814791254568302", null ],
      [ "coefficients_out", "structdh__filter__data.html#ace813b23f7a196778a1541ef8e1c3bfb", null ],
      [ "current_input_index", "structdh__filter__data.html#a5c93de77f7962004ff5a5a7d6837a93f", null ],
      [ "current_output_index", "structdh__filter__data.html#a8a7d7ca519cf149973643d4e830840c0", null ],
      [ "current_value", "structdh__filter__data.html#a3641fcb554f07d27cec89efbbdf75402", null ],
      [ "initialized", "structdh__filter__data.html#a490a9073cd7d07e171ffacbfcd920c5e", null ],
      [ "inputs", "structdh__filter__data.html#a9ee6125f263c6e9dfc73d65c30a8caf7", null ],
      [ "number_coefficients_in", "structdh__filter__data.html#a62d34ba6543c902e01e71461d0bf0037", null ],
      [ "number_coefficients_out", "structdh__filter__data.html#a585f468d1212f304d0cd212327828208", null ],
      [ "outputs", "structdh__filter__data.html#a26aa9039bc2fecfcce3bce9a0ccc8bec", null ]
    ] ],
    [ "DH_FILTER_RETURN_VALUE", "group__C-API.html#ga38f2ed9621e4192c47e69809da2f85bb", [
      [ "DH_FILTER_OK", "group__C-API.html#gga38f2ed9621e4192c47e69809da2f85bbacd71b4130d7239be99a64896d62da479", null ],
      [ "DH_FILTER_ERROR", "group__C-API.html#gga38f2ed9621e4192c47e69809da2f85bba9487b613850c1798b45d626694c94a9c", null ],
      [ "DH_FILTER_NO_DATA_STRUCTURE", "group__C-API.html#gga38f2ed9621e4192c47e69809da2f85bba1b434e02f0e62411d97781c86d03296c", null ],
      [ "DH_FILTER_DATA_STRUCTURE_NOT_INITIALIZED", "group__C-API.html#gga38f2ed9621e4192c47e69809da2f85bba97af70b3c1b4fda417dad7dea0b6cd56", null ],
      [ "DH_FILTER_UNKNOWN_FILTER_TYPE", "group__C-API.html#gga38f2ed9621e4192c47e69809da2f85bba7a50d4863a3b1dd95560a02cbc11bc73", null ],
      [ "DH_FILTER_ALLOCATION_FAILED", "group__C-API.html#gga38f2ed9621e4192c47e69809da2f85bbabb90353138e786f1af3a88f867ef6b49", null ]
    ] ],
    [ "DH_FILTER_TYPE", "group__C-API.html#ga221d0cc7d14748e8fc6bc81acba28644", [
      [ "DH_NO_FILTER", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a9d4a61964adf8e4141eddc194080be4b", null ],
      [ "DH_FIR_MOVING_AVERAGE_LOWPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a8252e1c602e1f5b4a6fcc879dadadc62", null ],
      [ "DH_FIR_MOVING_AVERAGE_HIGHPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a6389c24f45246fa0fcfa78cfb9055f82", null ],
      [ "DH_FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a3f2eefb3a0e5a8c24225a990c9e3aa14", null ],
      [ "DH_FIR_BRICKWALL_LOWPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a27e7e9bad7c6ee1801a92582734eb8a8", null ],
      [ "DH_FIR_BRICKWALL_HIGHPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a7958d7b66e5e9251c4043fabccc52733", null ],
      [ "DH_FIR_BRICKWALL_BANDPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a2dfb06bbc68d0fec4a1ba1a462e3563e", null ],
      [ "DH_FIR_BRICKWALL_BANDSTOP", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a276b9093a5ea6ac56cd6a63f3ce4ec82", null ],
      [ "DH_IIR_EXPONENTIAL_LOWPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a223195e5113b2516ef4380e54e15a332", null ],
      [ "DH_IIR_BUTTERWORTH_LOWPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644adb1d5c2a373594c79354e1787f0eb4ae", null ],
      [ "DH_IIR_BUTTERWORTH_HIGHPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a379243b4370d8ef7eba320518e4c34dc", null ],
      [ "DH_IIR_BUTTERWORTH_BANDPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a585eea5b30b129a9707c40bda25f967e", null ],
      [ "DH_IIR_BUTTERWORTH_BANDSTOP", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a3e75e61f34ca919999be21a5aaccb3ea", null ],
      [ "DH_IIR_CHEBYSHEV_LOWPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644ad1fb8757bc9b2411f57402158501cd5c", null ],
      [ "DH_IIR_CHEBYSHEV_HIGHPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644aecd65cac0ea14900366bbfb235abf85d", null ],
      [ "DH_IIR_CHEBYSHEV_BANDPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a97ac31ff1ebc6c1c39911a2c2a078d0a", null ],
      [ "DH_IIR_CHEBYSHEV_BANDSTOP", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644adc503975904a3798d7e9e40523c50a7d", null ],
      [ "DH_IIR_CHEBYSHEV2_LOWPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644ad2e80a2c83668955aee7ea8c6a7390e6", null ],
      [ "DH_IIR_CHEBYSHEV2_HIGHPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644ae5dd0e77881db950976186d0b60fe245", null ],
      [ "DH_IIR_CHEBYSHEV2_BANDPASS", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a9b2bf617f8a9ccc3db67f122269d979a", null ],
      [ "DH_IIR_CHEBYSHEV2_BANDSTOP", "group__C-API.html#gga221d0cc7d14748e8fc6bc81acba28644a90d082267a0ee41a56e16e4a46859219", null ]
    ] ],
    [ "dh_create_filter", "group__C-API.html#ga1e8cd07f95a961b06f943bb850d82189", null ],
    [ "dh_filter", "group__C-API.html#ga70f3787dc545e33d43b86fea87abdc86", null ],
    [ "dh_filter_get_gain", "group__C-API.html#gae3d2bb667065a72ff58fa6dc8f6ad633", null ],
    [ "dh_filter_get_gain_at", "group__C-API.html#ga5d3d63f208b98b5aaa3025be72976a21", null ],
    [ "dh_filter_set_gain", "group__C-API.html#gaeb65b26e035605a494a446be0835c027", null ],
    [ "dh_free_filter", "group__C-API.html#ga722f7f8a4f3eecbbe6e9cd6abd5bb1f3", null ],
    [ "dh_initialize_filter", "group__C-API.html#ga91a551637fcf030bede6805907d85511", null ]
];