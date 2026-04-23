# Shell Refinement Follow-up Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Finish the next prototype-alignment pass for the desktop shell without waiting for more user direction.

**Architecture:** Keep the existing Qt Widgets structure and make only targeted shell-level adjustments. Use TDD for each visual behavior change so layout regressions remain covered by `MainWindowTests` or `WidgetPanelSmokeTests`.

**Tech Stack:** C++17, Qt 6 Widgets, CMake, Qt Test

---

### Task 1: Maximized Startup And Right Control Column Resilience

**Files:**
- Modify: `src/app/MainWindow.cpp`
- Modify: `tests/MainWindowTests.cpp`

**Step 1:** Write failing tests for default maximized startup and a scrollable right control column.

**Step 2:** Run `MainWindowTests` and verify both tests fail.

**Step 3:** Set the main window to start maximized and wrap the right control stack in a `QScrollArea` with horizontal scrolling disabled.

**Step 4:** Run `MainWindowTests` again and verify the new tests pass.

**Step 5:** Run the full Qt build and test suite, then commit and push.

### Task 2: Left Rail Prototype Density

**Files:**
- Modify: `src/app/MainWindow.cpp`
- Modify: `src/theme/Theme.cpp`
- Modify: `tests/MainWindowTests.cpp`

**Step 1:** Add a failing test that captures the tighter prototype-like left rail styling.

**Step 2:** Run the targeted test and verify it fails.

**Step 3:** Refine left rail button text, spacing, and active-state styling with minimal code changes.

**Step 4:** Re-run the targeted test and the full shell tests.

**Step 5:** Commit and push.

### Task 3: Main Panel Header Tightening

**Files:**
- Modify: `src/app/MainWindow.cpp`
- Modify: `tests/MainWindowTests.cpp`

**Step 1:** Add failing tests for compact header control sizing and title density in the camera, profile, and history panels.

**Step 2:** Run the targeted test and verify it fails.

**Step 3:** Tighten header margins, control heights, and spacing to align with the web prototype.

**Step 4:** Run `MainWindowTests`, `WidgetPanelSmokeTests`, and then the full suite.

**Step 5:** Commit and push.
