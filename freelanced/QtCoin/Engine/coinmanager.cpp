#include "coinmanager.h"
#include "Engine/global.h"
CoinManager::CoinManager()
{

}
QString CoinManager::GetRandomString(int length, QString chars)
{
    //https://stackoverflow.com/questions/18862963/qt-c-random-string-generation/18866593
    if (chars == "")
    {
        chars = STRING_TEMPLATE;
    }
    QString possibleCharacters(chars.toLatin1());
    const int randomStringLength = length; //12 // assuming you want random strings of 12 characters

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QString randomString;
    for (int i = 0; i < randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        possibleCharacters.remove(index, 1); //line.replace(QString(nextChar), QString(""));
        randomString.append(nextChar);
    }
    return randomString;
}

void CoinManager::combinationUtil(QString arr, int n, int r, int index, QString data, int i)
{
    if (m_nState)
        return;
    if (index == r)
    {
        m_Coins.append(data);
        if (m_Coins.count() >= m_nTotal)
        {
            m_nState = true;
        }
        qDebug() << m_Coins.count();
        return;
    }

    if (i >= n)
    {
        qDebug() << m_Coins.count();
        return;
    }
    data[index] = arr[i];
    combinationUtil(arr, n, r, index + 1, data, i + 1);
    combinationUtil(arr, n, r, index, data, i + 1);
}

void CoinManager::GenerateCoin(int nLength, int nTotal)
{
    m_Coins.clear();

    m_strCoinKey = GetRandomString(15, STRING_TEMPLATE);

    QString data;
    m_nTotal = nTotal;

    m_nState = false;
    m_nLength = nLength;

    for (int i = 0; i < m_nLength; i++)
    {
        data += " ";
    }
    combinationUtil(m_strCoinKey, m_strCoinKey.length(), m_nLength, 0, data, 0);

}
