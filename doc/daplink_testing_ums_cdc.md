### <a name="testing_target"></a>Testing Target UMS/CDC

Test suite performs intense and detailed experimentation by putting a dedicated agent firmware (so called "Target Firmware" or "UART Application") onto tested Target.
With a little help of simple protocol all Target functions and Endpoints can be tested efficiently in a smart way.

For supported mbed boards that are registered on [mbed.org][mbed], Target Firmware Test Agent is built automatically on-the-fly using RESTful API of the [ARM mbed Online Compiler][mbed], then downloaded locally and flashed into Target memory.
However, in order to use this feature you need to specify your mbed.org user id and password via `--user` and `--password` parameters.

For boards that are not mbed enabled or not registered on [mbed.org][mbed], it is not possible to use ARM mbed Online Compiler, but test framework can still use pre-compiled binaries located at `--targetdir`. You need to create the binaries on your own, probably by adapting existing source code to match your Target/Board. **We encourage you to share back the results and register your hardware at [mbed.org][mbed]**.

**Note:** At the moment you can only choode one way of running the test suite, either using online compiler, or using binary firmware images, but not both.

**Note:** [DAPLink Target Validation][daplink_validation] source code is freely available at mbed mercurial repository.

[mbed]: https://developer.mbed.org "arm mbed developer resources"
[daplink_validation]: https://developer.mbed.org/users/c1728p9/code/daplink-validation/ "DAPLink Validation / Target Firmware Test Agent source code repository"
