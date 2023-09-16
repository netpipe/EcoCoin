#ifndef COINMANAGER_H
#define COINMANAGER_H
#include "Engine/global.h"

class CoinManager
{
public:
    CoinManager();
    QString         GetRandomString(int length, QString chars);
    void            GenerateCoin(int nLength, int nTotal);
    void            combinationUtil(QString arr, int n, int r, int index, QString data, int i);
    QList<QString>  GetRawCoins(){return m_Coins;}
private:
    QList<QString>  m_Coins;
    QString         m_strCoinKey;
    int             m_nTotal, m_nLength;
    bool            m_nState;
};

#endif // COINMANAGER_H
