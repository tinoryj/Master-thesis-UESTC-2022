#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

string config;

bool genConfig()
{
    ofstream output;
    output.open("config.json", ios::out);
    output << "{" << endl;
    output << "\"ChunkerConfig\": { " << endl;
    output << "    \"_chunkingType\": \"VariableSize\", " << endl;
    output << "    \"_minChunkSize\": 4096, " << endl;
    output << "    \"_avgChunkSize\": 8192, " << endl;
    output << "    \"_maxChunkSize\": 16384, " << endl;
    output << "    \"_slidingWinSize\": 48, " << endl;
    output << "    \"_ReadSize\": 256" << endl;
    output << "     }," << endl;
    output << " \"KeyServerConfig\" : { " << endl;
    output << "     \"_keyBatchSize\" : 100, " << endl;
    output << "     \"_keyEnclaveThreadNumber\" : 1, " << endl;
    output << "     \"_keyServerRArequestPort\" : 2559, " << endl;
    output << "     \"_keyServerIP\" : [\"192.168.3.4\"], " << endl;
    output << "     \"_keyServerPort\" : [6003], " << endl;
    output << "     \"_keyRegressionMaxTimes\" : 1048576, " << endl;
    output << "     \"_keyRegressionIntervals\" : 25920000 " << endl;
    output << " }, " << endl;
    output << "\"SPConfig\": { " << endl;
    output << "    \"_storageServerIP\": [ " << endl;
    output << "        \"192.168.3.4\" " << endl;
    output << "     ], " << endl;
    output << "    \"_storageServerPort\": [ " << endl;
    output << "        6001 " << endl;
    output << "     ], " << endl;
    output << "    \"_POWServerPort\": 6002, " << endl;
    output << "    \"_maxContainerSize\": 8388608 " << endl;
    output << "     }," << endl;
    output << "\"server\": { " << endl;
    output << "    \"_RecipeRootPath\": \"Recipes/\", " << endl;
    output << "    \"_containerRootPath\": \"Containers/\", " << endl;
    output << "    \"_fp2ChunkDBName\": \"db1\", " << endl;
    output << "    \"_fp2MetaDBame\": \"db2\", " << endl;
    output << "    \"_raSessionKeylifeSpan\": 259200000 " << endl;
    output << "     }," << endl;
    output << "\"client\": { " << endl;
    output << "    \"_clientID\": 1, " << endl;
    output << "    \"_sendChunkBatchSize\": 100, " << endl;
    output << "    \"_sendRecipeBatchSize\": 100000, " << endl;
    output << "    \"_POWBatchSize\": 100, " << endl;
    output << "    \"_keySeedFeatureNumber\" : 12, " << endl;
    output << "    \"_keySeedSuperFeatureNumber\" : 3," << endl;
    output << "    \"_keySeedGenFeatureThreadNumber\" : 3 " << endl;
    output << "     }" << endl;
    output << "}" << endl;
    output.close();
    return true;
}

bool genConfigFirst()
{
    ofstream output;
    output.open("config.json", ios::out);
    output << "{" << endl;
    output << "\"ChunkerConfig\": { " << endl;
    output << "    \"_chunkingType\": \"VariableSize\", " << endl;
    output << "    \"_minChunkSize\": 4096, " << endl;
    output << "    \"_avgChunkSize\": 8192, " << endl;
    output << "    \"_maxChunkSize\": 16384, " << endl;
    output << "    \"_slidingWinSize\": 48, " << endl;
    output << "    \"_ReadSize\": 256" << endl;
    output << "     }," << endl;
    output << " \"KeyServerConfig\" : { " << endl;
    output << "     \"_keyBatchSize\" : 100, " << endl;
    output << "     \"_keyEnclaveThreadNumber\" : 1, " << endl;
    output << "     \"_keyServerRArequestPort\" : 2559, " << endl;
    output << "     \"_keyServerIP\" : [\"192.168.3.4\"], " << endl;
    output << "     \"_keyServerPort\" : [6003], " << endl;
    output << "     \"_keyRegressionMaxTimes\" : 1048576, " << endl;
    output << "     \"_keyRegressionIntervals\" : 25920000 " << endl;
    output << " }, " << endl;
    output << "\"SPConfig\": { " << endl;
    output << "    \"_storageServerIP\": [ " << endl;
    output << "        \"192.168.3.4\" " << endl;
    output << "     ], " << endl;
    output << "    \"_storageServerPort\": [ " << endl;
    output << "        6001 " << endl;
    output << "     ], " << endl;
    output << "    \"_POWServerPort\": 6002, " << endl;
    output << "    \"_maxContainerSize\": 8388608 " << endl;
    output << "     }," << endl;
    output << "\"server\": { " << endl;
    output << "    \"_RecipeRootPath\": \"Recipes/\", " << endl;
    output << "    \"_containerRootPath\": \"Containers/\", " << endl;
    output << "    \"_fp2ChunkDBName\": \"db1\", " << endl;
    output << "    \"_fp2MetaDBame\": \"db2\", " << endl;
    output << "    \"_raSessionKeylifeSpan\": 259200000 " << endl;
    output << "     }," << endl;
    output << "\"client\": { " << endl;
    output << "    \"_clientID\": 1, " << endl;
    output << "    \"_sendChunkBatchSize\": 100, " << endl;
    output << "    \"_sendRecipeBatchSize\": 100000, " << endl;
    output << "    \"_POWBatchSize\": 100, " << endl;
    output << "    \"_keySeedFeatureNumber\" : 4, " << endl;
    output << "    \"_keySeedSuperFeatureNumber\" : 1," << endl;
    output << "    \"_keySeedGenFeatureThreadNumber\" : 3 " << endl;
    output << "     }" << endl;
    output << "}" << endl;
    output.close();
    return true;
}

bool keyGen()
{
    string cmdDir = "mkdir key";
    system(cmdDir.c_str());
    ofstream output;
    output.open("key/ca.crt", ios::out);
    output << "-----BEGIN CERTIFICATE-----" << endl;
    output << "MIIDwzCCAqugAwIBAgIURVFwAZ/10aXW0o3olCLwfOJCQc4wDQYJKoZIhvcNAQEL" << endl;
    output << "BQAwcTELMAkGA1UEBhMCQ04xCzAJBgNVBAgMAkdEMQswCQYDVQQHDAJTWjEMMAoG" << endl;
    output << "A1UECgwDQ09NMQwwCgYDVQQLDANOU1AxCzAJBgNVBAMMAkNBMR8wHQYJKoZIhvcN" << endl;
    output << "AQkBFhB5b3VyZW1haWxAcXEuY29tMB4XDTIxMTEwMjEzMDg1NloXDTIyMTEwMjEz" << endl;
    output << "MDg1NlowcTELMAkGA1UEBhMCQ04xCzAJBgNVBAgMAkdEMQswCQYDVQQHDAJTWjEM" << endl;
    output << "MAoGA1UECgwDQ09NMQwwCgYDVQQLDANOU1AxCzAJBgNVBAMMAkNBMR8wHQYJKoZI" << endl;
    output << "hvcNAQkBFhB5b3VyZW1haWxAcXEuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8A" << endl;
    output << "MIIBCgKCAQEArUpJcJNf1MPNBDD06+Dg71dYkAbHl+q5wGu9qTtPJgKxvIwD9XtM" << endl;
    output << "kd3WTDNOs5o6lfyzJb1Jl9ZntinsLchU1dj+z8hsa6QgBZpREEM6taMQG2hyA5Df" << endl;
    output << "z6QxKN9vlL6nPltJAU3mywbCGrfJhR13pB/Yll3Idu9x1bwoftRe2mx+lzbvHXC4" << endl;
    output << "OwIUo87+U3VUNWqPPtcDBcSgNYuPeHMampfFpsiEb2sbExsp4bBV7icOaXwpBU5S" << endl;
    output << "tbKfMxSNw4/hPAA0o+drSrex+ZpsQTUSiuYtAoM4BoLLGtie5+V9F+CTR5bTg+Vm" << endl;
    output << "8RhkRXOfAbHH2yZ/ibvYOOoYVRoU+OhC0wIDAQABo1MwUTAdBgNVHQ4EFgQUQtP8" << endl;
    output << "HLRdP5hwWufayb7GQEEVkwQwHwYDVR0jBBgwFoAUQtP8HLRdP5hwWufayb7GQEEV" << endl;
    output << "kwQwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAEmJghfVI4RI6" << endl;
    output << "fV1R5aFe2Z1sESXstRvHONIP7VzZaNM7GFwVTJ9bbZQJKEHFJ9blXEndzjgU63jw" << endl;
    output << "vzwHPm9pOhToD+PCYG36r2V9ifQ0vcuIY5t3DHoJDr89uVab5vjabswm3zmeh9vN" << endl;
    output << "iyyPx1d1mSU+JLXcK6TaFh1SyOLTP6pJqQ17TaHvhOD5XvBuy8TWHoEhW2nrFepE" << endl;
    output << "7NF/cAFDqwlZRkuHmF5ODHCrQH9YMNb6NGw/Li/Z0Brwmx48JtYCJ1MtBFqdHEMh" << endl;
    output << "guRbo9+qpRVRB92YqTi196rFvi5jPaEyRMTBo9yBQpbaug3e7gyQd2Ryacbj6HiG" << endl;
    output << "d+uE1o2bZQ==" << endl;
    output << "-----END CERTIFICATE-----" << endl;
    output.close();

    output.open("key/client.crt", ios::out);
    output << "-----BEGIN CERTIFICATE-----" << endl;
    output << "MIIDbTCCAlUCFFx8NTTizBx4CpgtUMES3R3OUcwBMA0GCSqGSIb3DQEBCwUAMHEx" << endl;
    output << "CzAJBgNVBAYTAkNOMQswCQYDVQQIDAJHRDELMAkGA1UEBwwCU1oxDDAKBgNVBAoM" << endl;
    output << "A0NPTTEMMAoGA1UECwwDTlNQMQswCQYDVQQDDAJDQTEfMB0GCSqGSIb3DQEJARYQ" << endl;
    output << "eW91cmVtYWlsQHFxLmNvbTAeFw0yMTExMDIxMzA5MzFaFw0yMjExMDIxMzA5MzFa" << endl;
    output << "MHUxCzAJBgNVBAYTAkNOMQswCQYDVQQIDAJHRDELMAkGA1UEBwwCU1oxDDAKBgNV" << endl;
    output << "BAoMA0NPTTEMMAoGA1UECwwDTlNQMQ8wDQYDVQQDDAZDTElFTlQxHzAdBgkqhkiG" << endl;
    output << "9w0BCQEWEHlvdXJlbWFpbEBxcS5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAw" << endl;
    output << "ggEKAoIBAQDqV4xX4Stg6Dpy6n6l+zI4qVwrjtW97Soi+6hZ26pgOTHFN42c5K9x" << endl;
    output << "HSvUnXJVClw0t9a9UoCUEir5WJD6FvDXHFGEFgRy7hBXUtqmZnHAGPTA2hupjc1P" << endl;
    output << "U4pmX4pX4GxxyEQVY7RA/sXZRFIMw7nq13iP9KjXbh1YZs3kC7lPLRHtnjwGhABx" << endl;
    output << "TzpbEGTAoTK2vOBsfT8jazVdZchu8tKUrEP58rSs4ImnwlWcUhubfzJam4lwjvOA" << endl;
    output << "A0ILuwQersR+FVEz3wLp7cpAUj2LVTytCoom+OYk8I2B3B/eT3T4P/nQEFRekoS+" << endl;
    output << "Y9YfqMuyhnkgWap/Ua6Y2KmGa/mr1TW7AgMBAAEwDQYJKoZIhvcNAQELBQADggEB" << endl;
    output << "AJhnZpDJ7va9yt7WvdtzUaeBysMo0MOL+BIOZy3oyphN4e9ozHyKOL/qZB6lFEDN" << endl;
    output << "Y5YeVC794pLkSRa4lNoE0Jah0Wq5LZo1yBWrOK7uQXUWWUeR86SY3NcJ0CtAplA8" << endl;
    output << "yTjFbaY+zwT8xj0Sk6wf7+WpRNwEezOeYwUfsIMzrtyd2a39UMva2GhTajoL9Rhz" << endl;
    output << "X5bVHdFGvJPmkV/sJQZ4OjbLDY1gZA3d+IRzpoa4BzQIdElEGmNI9jSZXLEtl6mY" << endl;
    output << "07/WweDe233jQ0x8qaSRiZfbtQxhhnyWji/Cka+PjwE0+0s9dPfk0PsfuXTk7wGc" << endl;
    output << "3lTDG2awmMEZyPVr7TcYEFA=" << endl;
    output << "-----END CERTIFICATE-----" << endl;
    output.close();

    output.open("key/client.pem", ios::out);
    output << "-----BEGIN RSA PRIVATE KEY-----" << endl;
    output << "MIIEpAIBAAKCAQEA6leMV+ErYOg6cup+pfsyOKlcK47Vve0qIvuoWduqYDkxxTeN" << endl;
    output << "nOSvcR0r1J1yVQpcNLfWvVKAlBIq+ViQ+hbw1xxRhBYEcu4QV1LapmZxwBj0wNob" << endl;
    output << "qY3NT1OKZl+KV+BscchEFWO0QP7F2URSDMO56td4j/So124dWGbN5Au5Ty0R7Z48" << endl;
    output << "BoQAcU86WxBkwKEytrzgbH0/I2s1XWXIbvLSlKxD+fK0rOCJp8JVnFIbm38yWpuJ" << endl;
    output << "cI7zgANCC7sEHq7EfhVRM98C6e3KQFI9i1U8rQqKJvjmJPCNgdwf3k90+D/50BBU" << endl;
    output << "XpKEvmPWH6jLsoZ5IFmqf1GumNiphmv5q9U1uwIDAQABAoIBAFTEGlXncyN4nTvY" << endl;
    output << "KrurY30vddGjtxkeYrGIylrGpJht19z4vVbVOcj2vlYIJcUxHC1NmnWdFDl7YOQU" << endl;
    output << "70wnZDLLYYkf1bgk+PA9Xi32vIq5/D6OJpdsXammFFf3kzk6Sr/Vqxr5l8gy4Co5" << endl;
    output << "Flzbp7KVAl1AKJrUj/TcKvmkbAGBINCaqENvaMDUUeuKC9+pLxif0Q3C7mbLCmNO" << endl;
    output << "yaACXEIa8iXsSqtGCTMu52rg8UJTH5L9+JM30qB7Xw2l4/4ZwhZcKuMth9ooPI9f" << endl;
    output << "YSw/eIH5vsQtsVKMVvC2yW4vI0oI67lyojI1PkaBZ3YyNKdDz6L0a6LuOnodwGUD" << endl;
    output << "A1nGafECgYEA+jbIRFxhXDU1NfLh96NZyyI5Bruaa+JEywV8unRgxIu7wFO1XpcV" << endl;
    output << "E1/Pne7ZEOmEQkBFn+0a04SjAm8Gwypj/2mQYHGeNuo3I4w8wwkwMMy1/QtsQjmV" << endl;
    output << "zvhwltVnMqdcY+W2P6NJD1AVyDJafW64SYhmM/sMZmjnz+YSXpb24O8CgYEA78LO" << endl;
    output << "h0Sg8a6qvJhucuJZMW+uRanYZEQM2jH4xI8T8Ra0un+pNh/7O0MsgcsO3W8WXktN" << endl;
    output << "jvZUHlZTJMFCWamQ5W67I0MD7D94NDIQrYxag8ArE0DHenhp/3hnYw4guL8JO719" << endl;
    output << "OYD1AXPsfzvh9iEoSFurr80kt/70TN1TysLdH/UCgYEAlZfXF/fbRZOSIpT5wGJr" << endl;
    output << "NuVZDstuwX1f/7liHt+hUyDvuUbSsqDFOvYXXKcGI/RY3HsspTbOyRMNmlDizCA1" << endl;
    output << "9OgaJ28GVnKlUJ1xXnHJ3AMn8we3S8i95iXmumcP8drZg+g8k8N91KfevfhM3Z0q" << endl;
    output << "lNv1rrIzca7amRNGfELpiwUCgYBgC0yKWU6ToiGZDQLpmIycRh2soF4jxDLV0UDT" << endl;
    output << "FHGrmSnqr6sMGIGeeslAcSRSRebS/R1jkH+f63rA9X3rxwZZMiNa+8R9hetUFV4i" << endl;
    output << "919m+bsHqmJ+R/BGO2hHAOjQuQ4s1Tptp5/95f8t9MIOw7eMTNSYxvfXkRUyGVMw" << endl;
    output << "nSDRZQKBgQDjUehxqSb65NVciFiOl/8GIhHNBioDg8/naZGGZg2fmsmdacnvcVde" << endl;
    output << "+Oyj9U46GInXidKIIT5pHddJbJyqlNZqFDKIYKBDfHVLnmsDDkFts67QSBQ1t9pT" << endl;
    output << "P7Msi/JvV6t+dT6Mc1y+M53kEhJtRQPOdqj+rqb4hPDSc2Y42MvALQ==" << endl;
    output << "-----END RSA PRIVATE KEY-----" << endl;
    output.close();
    return true;
}

bool testFileGen()
{
    ofstream output;
    output.open("10M.file", ios::out);
    srand(1);
    for (auto j = 0; j < 10; j++) {
        char buffer[1024 * 1024];
        for (auto i = 0; i < 128 * 1024; i++) {
            uint64_t tempRand = rand();
            memcpy(buffer + i * sizeof(uint64_t), &tempRand, sizeof(uint64_t));
        }
        output.write(buffer, 1024 * 1024);
    }
    output.close();
    return true;
}

int main(int argc, char* argv[])
{
    int confType = atoi(argv[1]);
    if (confType == 2) {
        genConfig();
    } else if (confType == 1) {
        genConfigFirst();
    }
    keyGen();
    testFileGen();
    return 0;
}
