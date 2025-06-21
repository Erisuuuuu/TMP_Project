#include <QtTest>
#include "networkclient.h"

class NetworkClientTest : public QObject {
    Q_OBJECT

private slots:
    void test_singletonInstance();  // Тестируем singleton
};

void NetworkClientTest::test_singletonInstance() {
    NetworkClient& inst1 = NetworkClient::instance();
    NetworkClient& inst2 = NetworkClient::instance();

    // Проверка, что оба экземпляра — это один и тот же объект
    QVERIFY(&inst1 == &inst2);
}

QTEST_MAIN(NetworkClientTest)
#include "tst_networkclienttest.moc"
