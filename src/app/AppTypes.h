#pragma once

struct MachinePosition {
    int x {23889};
    int y {26359};
};

struct AppState {
    bool measuring {false};
    bool paused {false};
    double progress {0.0};
    int seq {1010};
    int sampleCount {5};
    int step {200};
    MachinePosition pos;
};
