cmake_minimum_required(VERSION 3.21)

project(QFileMatrix VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

qt_standard_project_setup()

add_library(qfilematrix_core
    src/domain/MeasurementSpec.cpp
    src/domain/MeasurementSimulator.cpp
    src/domain/ProfileData.cpp
)

target_include_directories(qfilematrix_core PUBLIC src)

target_link_libraries(qfilematrix_core PUBLIC Qt6::Core)

add_library(qfilematrix_models
    src/models/MeasurementTableModel.cpp
)

target_include_directories(qfilematrix_models PUBLIC src)

target_link_libraries(qfilematrix_models PUBLIC Qt6::Core Qt6::Gui qfilematrix_core)

add_library(qfilematrix_app
    src/app/AppController.cpp
)

target_include_directories(qfilematrix_app PUBLIC src)

target_link_libraries(qfilematrix_app PUBLIC Qt6::Core qfilematrix_core qfilematrix_models)

add_library(qfilematrix_widgets
 src/theme/Theme.cpp
 src/widgets/CameraViewWidget.cpp
 src/widgets/LedIndicatorWidget.cpp
 src/widgets/PanelHeaderWidget.cpp
 src/widgets/ProfileChartWidget.cpp
 src/widgets/StatCardWidget.cpp
 src/widgets/SensorGaugeWidget.cpp
 src/widgets/DpadControlWidget.cpp
 src/dialogs/ManualSampleDialog.cpp
 src/dialogs/AlarmCenterDialog.cpp
)

target_include_directories(qfilematrix_widgets PUBLIC src)

target_link_libraries(qfilematrix_widgets PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets qfilematrix_app)

add_library(qfilematrix_panels
    src/app/MainWindow.cpp
    src/panels/TopTitleBar.cpp
    src/panels/DeviceStatusBar.cpp
    src/panels/BottomStatusBar.cpp
    src/panels/HistoryPanel.cpp
    src/panels/SensorPanel.cpp
    src/panels/MeasureControlPanel.cpp
    src/panels/ServoControlPanel.cpp
)

target_include_directories(qfilematrix_panels PUBLIC src)

target_link_libraries(qfilematrix_panels PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets qfilematrix_widgets qfilematrix_models)

qt_add_executable(QFileMatrixApp
    src/main.cpp
    resources/qfilematrix.qrc
)

target_link_libraries(QFileMatrixApp PRIVATE Qt6::Widgets qfilematrix_core qfilematrix_models qfilematrix_app qfilematrix_widgets qfilematrix_panels)

include(CTest)
if(BUILD_TESTING)
    enable_testing()

    qt_add_executable(MeasurementSpecTests
        tests/MeasurementSpecTests.cpp
    )
    target_link_libraries(MeasurementSpecTests PRIVATE Qt6::Core Qt6::Test qfilematrix_core)
    add_test(NAME MeasurementSpecTests COMMAND $<TARGET_FILE:MeasurementSpecTests>)
    set_tests_properties(MeasurementSpecTests PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:Qt6::Core>;$ENV{PATH}")

    qt_add_executable(MeasurementSimulatorTests
        tests/MeasurementSimulatorTests.cpp
    )
    target_link_libraries(MeasurementSimulatorTests PRIVATE Qt6::Core Qt6::Test qfilematrix_core)
    add_test(NAME MeasurementSimulatorTests COMMAND $<TARGET_FILE:MeasurementSimulatorTests>)
    set_tests_properties(MeasurementSimulatorTests PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:Qt6::Core>;$ENV{PATH}")

    qt_add_executable(MeasurementTableModelTests
        tests/MeasurementTableModelTests.cpp
    )
    target_include_directories(MeasurementTableModelTests PRIVATE src)
    target_link_libraries(MeasurementTableModelTests PRIVATE Qt6::Core Qt6::Gui Qt6::Test qfilematrix_core qfilematrix_models)
    add_test(NAME MeasurementTableModelTests COMMAND $<TARGET_FILE:MeasurementTableModelTests>)
    set_tests_properties(MeasurementTableModelTests PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:Qt6::Core>;$ENV{PATH}")

    qt_add_executable(AppControllerTests
        tests/AppControllerTests.cpp
    )
    target_include_directories(AppControllerTests PRIVATE src)
    target_link_libraries(AppControllerTests PRIVATE Qt6::Core Qt6::Test qfilematrix_core qfilematrix_models qfilematrix_app)
    add_test(NAME AppControllerTests COMMAND $<TARGET_FILE:AppControllerTests>)
    set_tests_properties(AppControllerTests PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:Qt6::Core>;$ENV{PATH}")

    qt_add_executable(WidgetPanelSmokeTests
        tests/WidgetPanelSmokeTests.cpp
    )
    target_include_directories(WidgetPanelSmokeTests PRIVATE src)
    target_link_libraries(WidgetPanelSmokeTests PRIVATE Qt6::Core Qt6::Widgets Qt6::Test qfilematrix_core qfilematrix_models qfilematrix_app qfilematrix_widgets qfilematrix_panels)
    add_test(NAME WidgetPanelSmokeTests COMMAND $<TARGET_FILE:WidgetPanelSmokeTests>)
    set_tests_properties(WidgetPanelSmokeTests PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:Qt6::Core>;$ENV{PATH}")

    qt_add_executable(VisualizationWidgetTests
        tests/VisualizationWidgetTests.cpp
    )
    target_include_directories(VisualizationWidgetTests PRIVATE src)
    target_link_libraries(VisualizationWidgetTests PRIVATE Qt6::Core Qt6::Widgets Qt6::Test qfilematrix_core qfilematrix_widgets)
    add_test(NAME VisualizationWidgetTests COMMAND $<TARGET_FILE:VisualizationWidgetTests>)
    set_tests_properties(VisualizationWidgetTests PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:Qt6::Core>;$ENV{PATH}")

    qt_add_executable(MainWindowTests
        tests/MainWindowTests.cpp
    )
    target_include_directories(MainWindowTests PRIVATE src)
    target_link_libraries(MainWindowTests PRIVATE Qt6::Core Qt6::Widgets Qt6::Test qfilematrix_core qfilematrix_models qfilematrix_app qfilematrix_widgets qfilematrix_panels)
    add_test(NAME MainWindowTests COMMAND $<TARGET_FILE:MainWindowTests>)
    set_tests_properties(MainWindowTests PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:Qt6::Core>;$ENV{PATH}")
endif()
