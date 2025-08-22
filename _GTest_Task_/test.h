#include <gtest/gtest.h>
#include <gmock/gmock.h>


class iDBconnection
{
public:
    iDBconnection() {};
    virtual ~iDBconnection() {};
  
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool execQuery(const std::string& str) = 0;
private:
};

class DBconnection : public iDBconnection
{
public:
    DBconnection();

    virtual ~DBconnection();
   
    virtual bool open() override;
   

    virtual void close() override;
   
    virtual bool execQuery(const std::string& str) override;
   

private:
    int* descriptor;
};


class ClassThatUsesDB
{
public:
    ClassThatUsesDB(iDBconnection* conn);

    bool openConnection();
    

    bool useConnection(const std::string& str);
    

    void closeConnection();
    

private:
    iDBconnection* m_conn;
};

class MockDBconnection : public iDBconnection
{
public:
    MOCK_METHOD(bool, open, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(bool, execQuery, (const std::string& str), (override));
};

class SomeTest : public ::testing::Test
{
protected:
    void SetUp();
  

    void TearDown();
   

protected:
    iDBconnection* idbconn;
    ClassThatUsesDB* dbconn;
};

