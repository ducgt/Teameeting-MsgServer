#include "test_client_cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include "test_client_common.h"


TestClientCmd::TestClientCmd()
{
    xClient.Init(3);
    RedisNode RedisList1[3] = {
        {0, "192.168.7.207", 6379, "", 2, 5, 0},
        {1, "192.168.7.207", 6379, "", 2, 5, 0},
        {2, "192.168.7.207", 6379, "", 2, 5, 0}
    };

    xClient.ConnectRedisCache(RedisList1, 3, CACHE_TYPE_1);
}

TestClientCmd::~TestClientCmd()
{
    xClient.Release();
}

void TestClientCmd::test_zadd()
{
    const char* charkey = "test_zadd";
    std::string strkey = charkey;
    std::string strValue;
    VALUES vVal;
    int64_t retVal=0;
    int64_t scores = 168;
    vVal.push_back(toString(scores));
    vVal.push_back(strValue);
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(charkey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.zadd(dbi, strkey, vVal, retVal)) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_set(const char* strkey, const char* strValue)
{
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(strkey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.set(dbi, strkey, strValue)) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_append()
{
    test_set("test", "hello");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.append(dbi, szKey, " xsky")) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_decr()
{
    test_set("test", "100");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t res = 0;
        if (xClient.decr(dbi, szKey, res)) {
            if (res == 99) {
                printf("%s success %ld \r\n", __PRETTY_FUNCTION__, res);
            } else {
                printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_decrby()
{
    test_set("test", "100");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t res = 0;
        if (xClient.decrby(dbi, szKey, 11, res)) {
            if (res == 89) {
                printf("%s success %ld \r\n", __PRETTY_FUNCTION__, res);
            } else {
                printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_incr()
{
    test_set("test", "100");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t res = 0;
        if (xClient.incr(dbi, szKey, res)) {
            if (res == 101) {
                printf("%s success %ld \r\n", __PRETTY_FUNCTION__, res);
            } else {
                printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_incrby()
{
    test_set("test", "100");
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t res = 0;
        if (xClient.incrby(dbi, szKey, 11, res)) {
            if (res == 111) {
                printf("%s success %ld \r\n", __PRETTY_FUNCTION__, res);
            } else {
                printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_get()
{

    char szKey[256] = {0};
    {
        strcpy(szKey, "test");
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
        if (bRet) {
            string strData;
            if (xClient.get(dbi, szKey, strData)) {
                printf("%s success data:%s \r\n", __PRETTY_FUNCTION__, strData.c_str());
            } else {
                printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
    }


    {
        sprintf(szKey, "test_%u", (unsigned int)time(NULL));
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
        if (bRet) {
            string strData;
            if (xClient.get(dbi, szKey, strData)) {
                printf("%s error data:%s \r\n", __PRETTY_FUNCTION__, strData.c_str());
            } else {
                printf("%s success [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
    }
}

void TestClientCmd::test_type()
{
    char szKey[256] = {0};

    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);

    if (bRet) {
        xClient.set(dbi, szKey, "wwwwwwwwwwwwwwwwwwwwwwwww");
        string strData;
        if (xClient.type(dbi, szKey, strData)) {
            printf("%s success data:%s \r\n", __PRETTY_FUNCTION__, strData.c_str());
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }

}

void TestClientCmd::test_getrange()
{
    test_set("test", "01234567890123456789");
    char szKey[256] = {0};

    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        string strData;
        if (xClient.getrange(dbi, szKey, 2, 6, strData)) {
            printf("%s success data:%s \r\n", __PRETTY_FUNCTION__, strData.c_str());
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_exists()
{
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.exists(dbi, szKey)) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_del()
{
    char szKey[256] = {0};
    strcpy(szKey, "test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.del(dbi, szKey)) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_mset()
{
    char szKey[256] = {0};
    DBIArray vdbi;
    VDATA    kvData;

    for (int i = 0; i < 10; i++) {
        RedisDBIdx dbi(&xClient);
        sprintf(szKey, "mset_key_%d", i);
        dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
        vdbi.push_back(dbi);
        kvData.push_back(szKey);
        sprintf(szKey, "mset_value_%d", i);
        kvData.push_back(szKey);
    }

    if (xClient.mset(vdbi, kvData)) {
        printf("%s success \r\n", __PRETTY_FUNCTION__);
    } else {
        printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, "mset error");
    }
}

void TestClientCmd::test_mget()
{
    char szKey[256] = {0};
    DBIArray vdbi;
    KEYS     kData;
    ReplyData Reply;
    for (int i = 0; i < 15; i++) {
        RedisDBIdx dbi(&xClient);
        sprintf(szKey, "mset_key_%d", i);
        dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
        vdbi.push_back(dbi);
        kData.push_back(szKey);
    }

    if (xClient.mget(vdbi, kData, Reply)) {
        printf("%s success %zu \r\n", __PRETTY_FUNCTION__, Reply.size());
        ReplyData::iterator iter = Reply.begin();
        for (; iter != Reply.end(); iter++) {
            printf("%d\t%s\r\n", (*iter).type, (*iter).str.c_str());
        }
    } else {
        printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, "mset error");
    }
}

void TestClientCmd::test_hset()
{
    char szHKey[256] = {0};
    strcpy(szHKey, "hashtest");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t count = 0;
        if (xClient.hset(dbi, szHKey, "filed1", "filed1_values", count)) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}


void TestClientCmd::test_lpush()
{
    char szHKey[256] = { 0 };
    strcpy(szHKey, "list_test");
    RedisDBIdx dbi(&xClient);

    VALUES vVal;
    vVal.push_back(toString(time(NULL)));
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t count = 0;
        if (xClient.lpush(dbi, szHKey, vVal, count)) {
            printf("%s success %ld \r\n", __PRETTY_FUNCTION__, count);
        }
        else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_llen()
{
    char szHKey[256] = { 0 };
    strcpy(szHKey, "list_test");
    RedisDBIdx dbi(&xClient);

    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t count = 0;
        if (xClient.llen(dbi, szHKey, count)) {
            printf("%s success len: %ld \r\n", __PRETTY_FUNCTION__, count);
        }
        else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_lrange()
{
    char szHKey[256] = { 0 };
    strcpy(szHKey, "list_test");
    RedisDBIdx dbi(&xClient);

    VALUES vVal;

    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        ArrayReply Reply;
        if (xClient.lrange(dbi, szHKey, 0, -1, Reply)) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
            ReplyData::iterator iter = Reply.begin();
            for (; iter != Reply.end(); iter++) {
                printf("%d\t%s\r\n", (*iter).type, (*iter).str.c_str());
            }
        }
        else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_lpop()
{
    char szHKey[256] = { 0 };
    strcpy(szHKey, "list_test");
    RedisDBIdx dbi(&xClient);

    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        string strVal;
        if (xClient.lpop(dbi, szHKey, strVal)) {
            printf("%s success val: %s \r\n", __PRETTY_FUNCTION__, strVal.c_str());
        }
        else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_rpop()
{
    char szHKey[256] = { 0 };
    strcpy(szHKey, "list_test");
    RedisDBIdx dbi(&xClient);

    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        string strVal;
        if (xClient.rpop(dbi, szHKey, strVal)) {
            printf("%s success val: %s \r\n", __PRETTY_FUNCTION__, strVal.c_str());
        }
        else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}


void TestClientCmd::test_publish()
{
    char szHKey[256] = { 0 };
    strcpy(szHKey, "pubsub_test");
    RedisDBIdx dbi(&xClient);

    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t count;
        if (xClient.publish(dbi, szHKey, "test message", count)) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_subscribe()
{
    char szHKey[256] = { 0 };
    strcpy(szHKey, "pubsub_test");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (!bRet) {
        return;
    }

    VDATA channels;
    channels.push_back(szHKey);
    xRedisContext ctx;
    if (xClient.subscribe(dbi, channels, ctx)) {
        printf("%s success \r\n", __PRETTY_FUNCTION__);
        ReplyData vReply;
        while (0 == xRedisClient::GetReply(&ctx, vReply)) {
            ReplyData::iterator iter = vReply.begin();
            for (; iter != vReply.end(); iter++) {
                printf("%d\t%s\r\n", (*iter).type, (*iter).str.c_str());
            }
        }

    } else {
        printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
    }
    xClient.unsubscribe(dbi, channels, ctx);
    xClient.FreexRedisContext(&ctx);
}

void TestClientCmd::test_sadd()
{
    char szHKey[256] = {0};
    strcpy(szHKey, "setadd");
    VALUES vValue;
    vValue.push_back("setaddvalue1");
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        int64_t count = 0;
        if (xClient.sadd(dbi, szHKey, vValue, count)) {
            printf("%s success \r\n", __PRETTY_FUNCTION__);
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}

void TestClientCmd::test_smembers()
{
    char szHKey[256] = {0};
    strcpy(szHKey, "setadd");
    VALUES vValue;
    RedisDBIdx dbi(&xClient);
    bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
    if (bRet) {
        if (xClient.smembers(dbi, szHKey, vValue)) {
            printf("%s success, vValue.size:%lu \r\n", __PRETTY_FUNCTION__, vValue.size());
            for (int i=0;i<(int)vValue.size();i++)
            {
                 printf("smembers value i: %d value is:%s\n", i, vValue[i].c_str());
            }
        } else {
            printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
        }
    }
}
