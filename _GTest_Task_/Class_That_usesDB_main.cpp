#include "test.h"
#include <iostream>

using namespace std;

    DBconnection::DBconnection()
    {
        cout << "constructor " << this << endl;
        descriptor = nullptr;
    }

    DBconnection::~DBconnection()
    {
        cout << "destructor " << this << endl;
        delete descriptor;
    }

    bool  DBconnection::open()
    {
        if ((descriptor == nullptr) || (*descriptor < 0))
        {
            srand(111);
            descriptor = new int((rand() % 10000) + 1);
            return true;
        }
        else 
            return false;
    }

    void  DBconnection::close()
    {
        if (descriptor == nullptr) return;
        *descriptor = -1;
    }

    bool  DBconnection::execQuery(const string& str)
    {
        if (descriptor == nullptr) return false;
        else if (*descriptor < 0) return false;
        else
        {
            cout << str << endl;
            return true;
        }
    }


    ClassThatUsesDB::ClassThatUsesDB(iDBconnection* conn)
    {
        m_conn = conn;
    }

    bool ClassThatUsesDB::openConnection()
    {
        if (m_conn->open())
        {
            cout << "OPEN" << endl;
            return true;
        }
        else
        {
            cout << "NOT OPEN" << endl;
            return false;
        }
    }

    bool ClassThatUsesDB::useConnection(const string& str)
    {
        if (m_conn->execQuery(str))
        {
            cout << "EXECECUTE" << endl;
            return true;
        }
        else
        {
            cout << "DO NOT EXECECUTE" << endl;
            return false;
        }
    }

    void ClassThatUsesDB::closeConnection()
    {
        m_conn->close();
    }

    void SomeTest::SetUp()
    {
        idbconn = new DBconnection();
        dbconn = new ClassThatUsesDB(idbconn);
    }

    void SomeTest::TearDown()
    {
        delete dbconn;
        delete idbconn;
    }


    TEST_F(SomeTest, test1)        //��֬�� "�߬� ���ܬ���ڬ�" ���֬լڬ߬֬߬ڬ� //  �ܬ�� �ӬѬ�ڬѬ߬� : ���� ���� �׽�Ʈ (�׽�Ʈ : �ܬ�߬Ԭݬڬ� ��� test(�Ѭ߬Ԭ�.))
    {
        bool test = dbconn->openConnection();
        bool reference(true);
        ASSERT_EQ(test, reference);
    }

    TEST_F(SomeTest, test2)        // 
    {
        bool test = dbconn->useConnection("123");
        bool reference(false);
        ASSERT_EQ(test, reference);
    }

    TEST_F(SomeTest, test3)        //��֬�� �٬Ѭ������ // ���� (�ܬ�߬Ԭݬڬ� ��� query(�Ѭ߬Ԭ�.)) �׽�Ʈ
    {
        dbconn->openConnection();
        bool test = dbconn->useConnection("123");
        bool reference(true);
        ASSERT_EQ(test, reference);
    }

    TEST_F(SomeTest, test4)        //��֬�� ���Ӭ���߬�Ԭ� ���ܬ���ڬ� // �ٽ� ���⿡ ���� �׽�Ʈ
    {
        dbconn->openConnection();
        bool test = dbconn->openConnection();
        bool reference(false);
        ASSERT_EQ(test, reference);
    }

    TEST_F(SomeTest, test5)        //��֬�� �߬� ��Ҭެ֬� �� �ڬ���ݬ�٬�ӬѬ߬ڬ֬� �ެ��-��Ҭ�֬ܬ��� // 'mock'�̶� ��ü�� ����� ��ȯ �׽�Ʈ
    {

        //���٬լѬ߬ڬ� �ެ��-��Ҭ�֬ܬ�� //'mock'�̶� ��ü â��
        MockDBconnection mdbc;

        //�٬Ѭ�ڬ��ӬѬ֬� ��جڬլѬ߬ڬ� // ������� �ۼ�
        EXPECT_CALL(mdbc, open).WillOnce(::testing::Return(true));
        //EXPECT_CALL(mdbc, open).WillOnce(::testing::Return(false));

        EXPECT_CALL(mdbc, execQuery("123")).WillOnce(::testing::Return(true));


        //�٬Ѭ���� ��Ҭ�ѬҬ��ܬ� �ѬݬԬ��ڬ�ެ� // �˰����� ó�� ����
        ClassThatUsesDB CTUDB(&mdbc);
        bool result1 = CTUDB.openConnection();
        bool result2 = CTUDB.useConnection("123");

        //���ѬӬ߬֬߬ڬ� ��֬٬�ݬ��Ѭ�� �� ��֬�֬�֬߬���  // ������ ��� ��
        ASSERT_EQ(true, result1);
        ASSERT_EQ(true, result2);
    }


    int main(int argc, char* argv[])
    {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
