#include "HelloForm.h"

HelloForm::HelloForm() {
    widget.setupUi(this);
    connect(widget.nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
}

HelloForm::~HelloForm() {
}

void HelloForm::textChanged(const QString& text) {
    if (0 < text.trimmed().length()) {
        widget.helloEdit->setText("Hello " + text.trimmed() + "!");
    } else {
        widget.helloEdit->clear();
    }
}
