### Changelog:

- **v0.7** - [NvApiOc v0.7](https://github.com/Demion/nvapioc/releases/download/v0.7/NvApiOc_v0.7.zip)
  * Add NVIDIA NvAPI switch option.
  * Add AMD ATI ADL switch option.
  * Add log file switch option.
- **v0.6** - [NvApiOc v0.6](https://github.com/Demion/nvapioc/releases/download/v0.6/NvApiOc_v0.6.zip)
  * Add curve (voltage:frequency) option. 
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

- **-core** gpuBusId pState frequencyKHz *(frequencyKHz: 0 = default; NVIDIA offset)*
- **-mem** gpuBusId pState frequencyKHz *(frequencyKHz: 0 = default; NVIDIA offset)*
- **-cvolt** gpuBusId pState voltageUV *(voltageUV: 0 = default)*
- **-mvolt** gpuBusId pState voltageUV *(voltageUV: 0 = default)*
- **-power** gpuBusId power *(power: AMD offset)*
- **-temp** gpuBusId priority tempC *(priority: 0 = false; 1 = true)*
- **-fan** gpuBusId fanIndex speed *(speed: -1 = default)*
- **-led** gpuBusId type brightness *(type: 0 = logo; 1 = sliBridge)*
- **-curve** gpuBusId count voltageUV frequencyKHz vUV2 fKHz2 vUV3 fKHz3
<br/>*(count: 0 = reset; -1 = save; frequencyKHz: 0 = default; NVIDIA offset)*
- **-nvidia** enable *(enable: 0 = false; 1 = true = default)*
- **-amd** enable *(enable: 0 = false; 1 = true = default)*
- **-log** enable *(enable: 0 = false; 1 = true = default)*
- **-restart**

### Credits:

- NVIDIA Inspector – Tool (Orbmu2k) https://orbmu2k.de/tools/nvidia-inspector-tool
- nvapi-rs - NVAPI bindings for Rust (arcnmx) https://github.com/arcnmx/nvapi-rs https://arcnmx.github.io/nvapi-rs/nvapi/
- OverdriveNTool - tool for AMD Hawaii, Fiji, Polaris, Vega GPUs (tede) https://forums.guru3d.com/threads/overdriventool-tool-for-amd-gpus.416116/