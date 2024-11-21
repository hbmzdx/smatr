#include "AlipaySignature.h"
#include <vector>
#include <random>
#include <string.h>
#include <algorithm>
#include <QDebug>
#include <QTimer>


std::string AlipaySignature::sign(RSA* pRsa, std::string content)
{
    if (!pRsa)
        return "";

    std::string result = "";
    const char* cstr = content.c_str();
    unsigned char hash[SHA256_DIGEST_LENGTH] = { 0 };
    SHA256((unsigned char*)cstr, strlen(cstr), hash);
    unsigned char* sig = new unsigned char[RSA_size(pRsa)];
    unsigned int nLen = 0;
    int iRet = RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, sig, &nLen, pRsa);
    if (iRet > 0) {
        for (unsigned int i = 0; i < nLen; i++) {
            result += sig[i];
        }
        result = AlipaySignature::base64Encode(result);
    }
    return result;
}

bool AlipaySignature::verifySign(RSA* pRsa, std::string content, std::string sign)
{
    if (!pRsa)
        return false;

    sign = AlipaySignature::base64Decode(sign);
    if ("" == sign)
        return false;

    const char* cstr = content.c_str();
    unsigned char hash[SHA256_DIGEST_LENGTH] = { 0 };
    SHA256((unsigned char*)cstr, strlen(cstr), hash);

    return RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, reinterpret_cast<const unsigned char*>(sign.c_str()), sign.length(), pRsa);
}

//RSA* SHA256Sign::createRSA()
//{
//	return  RSA_generate_key(2048, RSA_F4, 0, 0);
//}

RSA* AlipaySignature::getRsaFromFile(std::string filePath)
{
    OpenSSL_add_all_algorithms();
    BIO* in = BIO_new(BIO_s_file());
    BIO_read_filename(in, filePath.c_str());
    RSA* pRsa = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    return pRsa;
}

QImage AlipaySignature::show2DBarcode(QByteArray twoDBarcodeimUrl)
{
    //制作二维码，传入字符串，设定纠错级别
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(twoDBarcodeimUrl, qrcodegen::QrCode::Ecc::LOW);

    QImage QrCode_Image = QImage(qr.getSize(), qr.getSize(), QImage::Format_RGB888);
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y) == 0) {
                QrCode_Image.setPixel(x, y, qRgb(255, 255, 255));
            }else {
                QrCode_Image.setPixel(x, y, qRgb(0, 0, 0));
            }
        }
    }

//    //图像大小转换为标签的大小
//    qDebug()<<label->width()<<label->height();
//    QrCode_Image = QrCode_Image.scaled(label->size(), Qt::KeepAspectRatio);
//    //转换为二维码图形在Label中显示
//    label->setPixmap(QPixmap::fromImage(QrCode_Image));
    return QrCode_Image;
}

//std::string SHA256Sign::sortSignJson(std::string content)
//{
//	neb::CJsonObject oJson(content);
//	oJson = sortAllKeys(oJson);
//	std::vector<std::string> keys = oJson.GetAllKeys();
//	sort(keys.begin(), keys.end());
//	std::string sign = "";
//	for (auto key : keys) {
//		sign += "&";
//		sign += key;
//		sign += "=";
//		if (oJson.IsNum(key)) {
//			int val = 0;
//			oJson.Get(key, val);
//			sign += std::to_string(val);
//		}
//		else if (oJson.IsString(key)) {
//			std::string tmp = "";
//			oJson.Get(key, tmp);
//			sign += tmp;
//		}
//		else if (oJson.IsArray(key)) {
//			neb::CJsonObject data;
//			oJson.Get(key, data);
//			sign += data.ToString();
//		}
//		else if (oJson.IsObject(key)) {
//			neb::CJsonObject data;
//			oJson.Get(key, data);
//			sign += data.ToString();
//		}
//	}
//	if (sign.length())
//		sign = sign.substr(1);
//	return sign;
//}

//neb::CJsonObject SHA256Sign::sortAllKeys(neb::CJsonObject obj)
//{
//	neb::CJsonObject result;
//	std::vector<std::string> keys = obj.GetAllKeys();
//	sort(keys.begin(), keys.end());
//	for (auto key : keys) {
//		if (obj.IsNum(key) || obj.IsString(key)) {
//			result.Add(key, obj[key]);
//		}
//		else if (obj.IsArray(key)) {
//			result.AddEmptySubArray(key);
//			for (int i = 0; i < obj[key].GetArraySize(); i++) {
//				result[key].Add(sortAllKeys(obj[key][i]));
//			}
//		}
//		else if (obj.IsObject(key)) {
//			result.AddEmptySubObject(key);
//			result[key] = sortAllKeys(result[key]);
//		}
//	}
//	return result;
//}

std::string AlipaySignature::base64Encode(std::string content)
{
    BIO* bmem = NULL;
    BIO* b64 = NULL;
    BUF_MEM* bptr = NULL;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, reinterpret_cast<const unsigned char*>(content.c_str()), content.length());
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    std::string str = std::string(bptr->data, bptr->length);
    BIO_free_all(b64);
    return str;
}

std::string AlipaySignature::base64Decode(std::string content)
{
    std::string result = "";
    const char* cstr = content.c_str();
    BIO* bmem = NULL;
    BIO* b64 = NULL;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new_mem_buf((void*)cstr, strlen(cstr));
    b64 = BIO_push(b64, bmem);
    unsigned char sign_cstr[256] = { 0 };
    int iRet = BIO_read(b64, sign_cstr, 256);
    if (iRet > 0) {
        for (int i = 0; i < 256; i++) {
            result += sign_cstr[i];
        }
    }
    BIO_free_all(b64);
    return result;
}
