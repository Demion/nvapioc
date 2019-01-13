### Changelog:

- **v0.5** - [NvApiOc v0.5](https://github.com/Demion/nvapioc/releases/download/v0.5/NvApiOc_v0.5.zip)
  * Replace GPU index with bus id.
  * Reformat log file.
- **v0.4** - [NvApiOc v0.4](https://github.com/Demion/nvapioc/releases/download/v0.4/NvApiOc_v0.4.zip)
  * Add AMD ATI ADL Overdrive support.
  * Reformat voltage options.
  * Fix x64 GPU handle size.
- **v0.3** - [NvApiOc v0.3](https://github.com/Demion/nvapioc/releases/download/v0.3/NvApiOc_v0.3.zip)
  * Add require administrator rights (UAC).
  * Add x64 build support.
- **v0.2** - [NvApiOc v0.2](https://github.com/Demion/nvapioc/releases/download/v0.2/NvApiOc_v0.2.zip)
  * Add log file.
  * Add TCC devices support.
  * Add restart driver option.
  * Remove PState options *(not working as expected)*.
- **v0.1** - [NvApiOc v0.1](https://github.com/Demion/nvapioc/releases/download/v0.1/NvApiOc_v0.1.zip)

### About:

NvApiOc Open Source NVIDIA NvAPI & AMD ATI ADL Overdrive GPU Overclock

### Usage:

- **-core** gpuBusId pState frequencyKHz *(0 = default NVIDIA offset)*
- **-mem** gpuBusId pState frequencyKHz *(0 = default NVIDIA offset)*
- **-cvolt** gpuBusId pState voltageUV *(0 = default)*
- **-mvolt** gpuBusId pState voltageUV *(0 = default)*
- **-power** gpuBusId power *(AMD offset)*
- **-temp** gpuBusId priority tempC *(0 = false 1 = true)*
- **-fan** gpuBusId fanIndex speed *(-1 = auto)*
- **-led** gpuBusId type brightness *(0 = logo 1 = sliBridge)*
- **-restart**

### Donation:

- ETH: 0xcb4effdeb46479caa0fef5f5e3569e4852f753a2
- BTC: 1H1zNLHNxqtMgVYJESF6PjPVq2h9tLW4xG

### Credits:

- NVIDIA Inspector – Tool (Orbmu2k) https://orbmu2k.de/tools/nvidia-inspector-tool
- nvapi-rs - NVAPI bindings for Rust (arcnmx) https://github.com/arcnmx/nvapi-rs https://arcnmx.github.io/nvapi-rs/nvapi/
- OverdriveNTool - tool for AMD Hawaii, Fiji, Polaris, Vega GPUs (tede) https://forums.guru3d.com/threads/overdriventool-tool-for-amd-gpus.416116/