#include "dialogs/ManualSampleDialog.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "theme/Theme.h"

ManualSampleDialog::ManualSampleDialog(QWidget *parent)
: QDialog(parent)
{
    setWindowTitle(QStringLiteral("手动采样"));
    setMinimumWidth(460);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *headerLayout = new QHBoxLayout;
    headerLayout->setContentsMargins(12, 10, 12, 10);
    auto *titleLabel = new QLabel(QStringLiteral("手动采样"));
    titleLabel->setStyleSheet(QStringLiteral("font-size:13px;font-weight:600;color:%1;").arg(Theme::palette().text.name()));
    auto *closeButton = new QPushButton(QStringLiteral("✕"));
    closeButton->setObjectName(QStringLiteral("manualSampleCloseButton"));
    closeButton->setFlat(true);
    closeButton->setFixedSize(24, 24);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(QStringLiteral("QPushButton{border:none;color:%1;font-size:14px;}QPushButton:hover{color:%2;}")
        .arg(Theme::palette().textMuted.name(), Theme::palette().text.name()));
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeButton);
    mainLayout->addLayout(headerLayout);

    auto *divider = new QWidget;
    divider->setFixedHeight(1);
    divider->setStyleSheet(QStringLiteral("background:%1;").arg(Theme::palette().border.name()));
    mainLayout->addWidget(divider);

    auto *bodyLayout = new QVBoxLayout;
    bodyLayout->setContentsMargins(16, 16, 16, 16);
    bodyLayout->setSpacing(12);

    auto *descLabel = new QLabel(QStringLiteral("将执行一次手动采样，采样完成后数据自动入表。请确认十字准线已定位目标区域。"));
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(QStringLiteral("font-size:12px;color:%1;").arg(Theme::palette().textMuted.name()));
    bodyLayout->addWidget(descLabel);

    auto *grid = new QGridLayout;
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(6);
    const auto keyStyle = QStringLiteral("font-size:12px;color:%1;").arg(Theme::palette().textMuted.name());
    const auto valStyle = QStringLiteral("font-size:12px;color:%1;font-family:Consolas;").arg(Theme::palette().text.name());

    auto *posKey = new QLabel(QStringLiteral("当前位置"));
    posKey->setStyleSheet(keyStyle);
    m_posLabel = new QLabel(QStringLiteral("X 0.000 · Y 0.000"));
    m_posLabel->setStyleSheet(valStyle);
    auto *progKey = new QLabel(QStringLiteral("程序号"));
    progKey->setStyleSheet(keyStyle);
    m_programLabel = new QLabel(QStringLiteral("1010"));
    m_programLabel->setStyleSheet(valStyle);
    auto *batchKey = new QLabel(QStringLiteral("批号"));
    batchKey->setStyleSheet(keyStyle);
    m_batchLabel = new QLabel(QStringLiteral("3"));
    m_batchLabel->setStyleSheet(valStyle);

    grid->addWidget(posKey, 0, 0);
    grid->addWidget(m_posLabel, 0, 1);
    grid->addWidget(progKey, 1, 0);
    grid->addWidget(m_programLabel, 1, 1);
    grid->addWidget(batchKey, 2, 0);
    grid->addWidget(m_batchLabel, 2, 1);
    bodyLayout->addLayout(grid);

    auto *buttonRow = new QHBoxLayout;
    buttonRow->addStretch();
    auto *cancelButton = new QPushButton(QStringLiteral("取消"));
    cancelButton->setObjectName(QStringLiteral("manualSampleCancelButton"));
    cancelButton->setFixedHeight(28);
    m_confirmButton = new QPushButton(QStringLiteral("确认采样"));
    m_confirmButton->setObjectName(QStringLiteral("manualSampleConfirmButton"));
    m_confirmButton->setProperty("role", QStringLiteral("primary"));
    m_confirmButton->setFixedHeight(28);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_confirmButton, &QPushButton::clicked, this, [this] { emit sampleConfirmed(); accept(); });
    buttonRow->addWidget(cancelButton);
    buttonRow->addWidget(m_confirmButton);
    bodyLayout->addLayout(buttonRow);

    mainLayout->addLayout(bodyLayout);

    setStyleSheet(Theme::fieldStyle() + QStringLiteral(
        "QDialog{background:%1;border:1px solid %2;border-radius:8px;}"
        "QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:0 16px;color:%3;font-size:12px;}"
        "QPushButton:hover{background:%4;}"
        "QPushButton[role='primary']{background:%5;border-color:%5;color:white;}"
    ).arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(),
          Theme::palette().text.name(), Theme::palette().bgSunken.name(),
          Theme::palette().brand.name()));
}

void ManualSampleDialog::setPosition(const QString &x, const QString &y)
{
    m_posLabel->setText(QStringLiteral("X %1 · Y %2").arg(x, y));
}

void ManualSampleDialog::setProgram(const QString &program)
{
    m_programLabel->setText(program);
}

void ManualSampleDialog::setBatch(const QString &batch)
{
    m_batchLabel->setText(batch);
}

QString ManualSampleDialog::positionX() const
{
    return m_posLabel->text();
}

QString ManualSampleDialog::positionY() const
{
    return m_posLabel->text();
}

QString ManualSampleDialog::program() const
{
    return m_programLabel->text();
}

QString ManualSampleDialog::batch() const
{
    return m_batchLabel->text();
}
