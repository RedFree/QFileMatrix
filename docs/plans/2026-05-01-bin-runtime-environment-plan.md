# Bin Runtime Environment Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Add a dedicated ignored `bin/` runtime output with deployable `Debug` and `Release` app folders, plus stable Windows scripts for configure, build, deploy, and run.

**Architecture:** Keep CMake focused on building into `build/` with the Visual Studio generator, then use thin batch wrappers to configure MSVC, compile a chosen configuration, and deploy the built executable into `bin/<Config>/` with `windeployqt`. Normalize `.gitignore` and update `README.md` so the supported workflow is explicit and reproducible.

**Tech Stack:** Windows batch scripts, CMake, Visual Studio 2022, Qt 6 Widgets, `windeployqt`

---

### Task 1: Stabilize Configure And Build Entry Points

**Files:**
- Create: `configure_app.bat`
- Modify: `build_app.bat`

**Step 1:** Create `configure_app.bat` that calls `vcvars64.bat`, creates `build/` when needed, and runs:

```bat
cmake -S I:\century\QFileMatrix -B I:\century\QFileMatrix\build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=I:/workspace/mylibs/Qt6.6.3/6.6.3/msvc2019_64
```

**Step 2:** Update `build_app.bat` to accept `Debug` or `Release`, default to `Debug`, ensure the build tree is configured, and build only `QFileMatrixApp` with:

```bat
cmake --build I:\century\QFileMatrix\build --config Debug --target QFileMatrixApp
```

**Step 3:** Run `configure_app.bat` and verify CMake reports the `Visual Studio 17 2022` generator.

**Step 4:** Run `build_app.bat Debug` and verify `build\Debug\QFileMatrixApp.exe` exists.

### Task 2: Add Bin Deployment And Run Scripts

**Files:**
- Create: `deploy_app.bat`
- Create: `run_app.bat`

**Step 1:** Create `deploy_app.bat` that accepts `Debug` or `Release`, calls `build_app.bat`, clears `bin\<Config>` safely, copies `build\<Config>\QFileMatrixApp.exe`, and runs:

```bat
I:\workspace\mylibs\Qt6.6.3\6.6.3\msvc2019_64\bin\windeployqt.exe --dir I:\century\QFileMatrix\bin\Debug I:\century\QFileMatrix\bin\Debug\QFileMatrixApp.exe
```

Use `--debug` for `Debug` and `--release` for `Release`.

**Step 2:** Create `run_app.bat` that accepts `Debug` or `Release`, calls `deploy_app.bat`, and launches `bin\<Config>\QFileMatrixApp.exe` with `Start-Process`-equivalent batch behavior.

**Step 3:** Run `deploy_app.bat Debug` and verify `bin\Debug\QFileMatrixApp.exe` plus Qt DLLs/plugins exist.

**Step 4:** Run `deploy_app.bat Release` and verify `bin\Release\QFileMatrixApp.exe` exists if the release build succeeds.

### Task 3: Normalize Ignore Rules And Usage Docs

**Files:**
- Modify: `.gitignore`
- Modify: `README.md`

**Step 1:** Replace the corrupted `.gitignore` trailing content with normalized text and add:

```gitignore
bin/
```

**Step 2:** Update `README.md` to document the supported script workflow:
- `configure_app.bat`
- `build_app.bat Debug`
- `deploy_app.bat Debug`
- `run_app.bat Debug`

Clarify that `build/` holds compiler artifacts and `bin/` holds runnable packaged output.

**Step 3:** Confirm `git status --ignored` shows `bin/` as ignored after deployment.

### Task 4: End-To-End Verification

**Files:**
- Verify only

**Step 1:** Run `configure_app.bat`.

**Step 2:** Run `build_app.bat Debug`.

**Step 3:** Run `deploy_app.bat Debug`.

**Step 4:** Run `run_app.bat Debug` and confirm the app launches from `bin\Debug\QFileMatrixApp.exe`.

**Step 5:** Run `deploy_app.bat Release` and verify `bin\Release\QFileMatrixApp.exe` and Qt runtime files are produced.

**Step 6:** Review `git status` to ensure only intended script/docs/ignore changes remain tracked.
