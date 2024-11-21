#include <string>
#include "AliPaySignature/openssl/sha.h"
#include "AliPaySignature/openssl/evp.h"
#include "AliPaySignature/openssl/pem.h"
#include <AliPaySignature/openssl/err.h>
#include <AliPaySignature/openssl/rsa.h>
#include <QPainter>
#include <QImage>
#include <QClipboard>
#include <QFileDialog>
//#include "CJsonObject.hpp"
#include "AliPaySignature/qrcode/qrcodegen.h"
#include <QLabel>

class AlipaySignature
{
public:

    /*
     * 功  能：使用私钥对待签名内容进行签名
     * 参  数：
     * @pRsa:私钥数据，通过getRsaFromFile函数获取
     * @content:待签名内容
     * 返回值：签名后的数据
    */
    static std::string sign(RSA* pRsa,std::string content);

    static bool verifySign(RSA* pRsa, std::string content, std::string sign);

    //static RSA* createRSA();

    /*
     * 功  能：获取私钥
     * 参  数：
     * @filePath:私钥存放路径
     * 返回值：从文件中获取私钥
    */
    static RSA* getRsaFromFile(std::string filePath);

    /*
     * 功  能：将支付宝返回的二维码url转成二维码图片
     * @twoDBarcodeimUrl:支付宝返回的二维码url
     * 返回值：返回二维码图片数据
    */
    static QImage show2DBarcode(QByteArray twoDBarcodeimUrl);

    //static std::string sortSignJson(std::string content);

private:
    //static neb::CJsonObject sortAllKeys(neb::CJsonObject obj);

    static std::string base64Encode(std::string content);

    static std::string base64Decode(std::string content);

};
