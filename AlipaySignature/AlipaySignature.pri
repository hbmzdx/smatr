
LIBS += $$PWD/openssl/lib/libcrypto-1_1-x64.dll
LIBS += $$PWD/openssl/lib/libssl-1_1-x64.dll

include ($$PWD/openssl/openssl.pri)
include ($$PWD/qrcode/qrcode.pri)

SOURCES += \
        $$PWD/AlipayHandle.cpp \
        $$PWD/AlipaySignature.cpp

HEADERS += \
        $$PWD/AlipayHandle.h \
        $$PWD/AlipaySignature.h




