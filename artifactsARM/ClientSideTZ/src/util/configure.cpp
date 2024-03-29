//
// Created by a on 11/17/18.
//

#include "configure.hpp"

Configure::~Configure() { }
Configure::Configure() { }
Configure::Configure(std::string path)
{
    this->readConf(path);
}

void Configure::readConf(std::string path)
{
    using namespace boost;
    using namespace boost::property_tree;
    ptree root;
    read_json<ptree>(path, root);

    //Chunker Configure
    _chunkingType = root.get<std::string>("ChunkerConfig._chunkingType");
    _maxChunkSize = root.get<uint64_t>("ChunkerConfig._maxChunkSize");
    _minChunkSize = root.get<uint64_t>("ChunkerConfig._minChunkSize");
    _slidingWinSize = root.get<uint64_t>("ChunkerConfig._slidingWinSize");
    _averageChunkSize = root.get<uint64_t>("ChunkerConfig._avgChunkSize");
    _ReadSize = root.get<uint64_t>("ChunkerConfig._ReadSize");

    //Key Server Congigure
    _keyEnclaveThreadNumber = root.get<uint64_t>("KeyServerConfig._keyEnclaveThreadNumber");
    _keyBatchSize = root.get<uint64_t>("KeyServerConfig._keyBatchSize");
    _keyServerRArequestPort = root.get<int>("KeyServerConfig._keyServerRArequestPort");
    _keyServerIP.clear();
    for (ptree::value_type& it : root.get_child("KeyServerConfig._keyServerIP")) {
        _keyServerIP.push_back(it.second.data());
    }
    _keyServerPort.clear();
    for (ptree::value_type& it : root.get_child("KeyServerConfig._keyServerPort")) {
        _keyServerPort.push_back(it.second.get_value<int>());
    }
    _keyRegressionMaxTimes = root.get<uint32_t>("KeyServerConfig._keyRegressionMaxTimes");
    _keyRegressionIntervals = root.get<uint32_t>("KeyServerConfig._keyRegressionIntervals");

    //SP Configure
    _maxContainerSize = root.get<uint64_t>("SPConfig._maxContainerSize");
    _storageServerIP.clear();
    for (ptree::value_type& it : root.get_child("SPConfig._storageServerIP")) {
        _storageServerIP.push_back(it.second.data());
    }
    _storageServerPort.clear();
    for (ptree::value_type& it : root.get_child("SPConfig._storageServerPort")) {
        _storageServerPort.push_back(it.second.get_value<int>());
    }
    _POWServerPort = root.get<int>("SPConfig._POWServerPort");

    //client Configure
    _clientID = root.get<int>("client._clientID");
    _sendChunkBatchSize = root.get<int>("client._sendChunkBatchSize");
    _sendRecipeBatchSize = root.get<int>("client._sendRecipeBatchSize");
    _POWBatchSize = root.get<int>("client._POWBatchSize");
    _keySeedFeatureNumber = root.get<int>("client._keySeedFeatureNumber");
    _keySeedSuperFeatureNumber = root.get<int>("client._keySeedSuperFeatureNumber");
    _keySeedGenFeatureThreadNumber = root.get<int>("client._keySeedGenFeatureThreadNumber");
}

// chunking settings
string Configure::getChunkingType()
{

    return _chunkingType;
}

uint64_t Configure::getMaxChunkSize()
{

    return _maxChunkSize;
}

uint64_t Configure::getMinChunkSize()
{

    return _minChunkSize;
}

uint64_t Configure::getAverageChunkSize()
{

    return _averageChunkSize;
}

uint64_t Configure::getSlidingWinSize()
{

    return _slidingWinSize;
}

uint64_t Configure::getReadSize()
{
    return _ReadSize;
}

// key management settings
uint64_t Configure::getKeyEnclaveThreadNumber()
{
    return _keyEnclaveThreadNumber;
}

int Configure::getKeyBatchSize()
{
    return _keyBatchSize;
}

int Configure::getkeyServerRArequestPort()
{
    return _keyServerRArequestPort;
}

/*
std::vector<std::string> Configure::getkeyServerIP() {

    return _keyServerIP;
}

std::vector<int> Configure::getKeyServerPort() {

    return _keyServerPort;
}

*/

std::string Configure::getKeyServerIP()
{
    return _keyServerIP[0];
}

int Configure::getKeyServerPort()
{
    return _keyServerPort[0];
}

uint32_t Configure::getKeyRegressionMaxTimes()
{
    return _keyRegressionMaxTimes;
}

uint32_t Configure::getKeyRegressionIntervals()
{
    return _keyRegressionIntervals;
}
// storage management settings
std::string Configure::getStorageServerIP()
{

    return _storageServerIP[0];
}
/*
std::vector<std::string> Configure::getStorageServerIP() {

    return _storageServerIP;
}*/

int Configure::getStorageServerPort()
{

    return _storageServerPort[0];
}

/*
std::vector<int> Configure::getStorageServerPort() {

    return _storageServerPort;
}*/

uint64_t Configure::getMaxContainerSize()
{

    return _maxContainerSize;
}

//pow enclave settings

int Configure::getPOWQuoteType()
{
    return _POWQuoteType;
}

int Configure::getPOWIASVersion()
{
    return _POWIasVersion;
}

int Configure::getPOWServerPort()
{
    return _POWServerPort;
}

std::string Configure::getPOWEnclaveName()
{
    return _POWEnclaveName;
}

std::string Configure::getPOWSPID()
{
    return _POWSPID;
}

int Configure::getPOWIASServerType()
{
    return _POWIasServerType;
}

int Configure::getPOWBatchSize()
{
    return _POWBatchSize;
}
// km enclave settings
int Configure::getKMQuoteType()
{
    return _KMQuoteType;
}

int Configure::getKMIASVersion()
{
    return _KMIasVersion;
}

int Configure::getKMServerPort()
{
    return _KMServerPort;
}

std::string Configure::getKMEnclaveName()
{
    return _KMEnclaveName;
}

std::string Configure::getKMSPID()
{
    return _KMSPID;
}

int Configure::getKMIASServerType()
{
    return _KMIasServerType;
}

// client settings
int Configure::getClientID()
{
    return _clientID;
}

int Configure::getSendChunkBatchSize()
{
    return _sendChunkBatchSize;
}

int Configure::getKeySeedFeatureNumber()
{
    return _keySeedFeatureNumber;
}

int Configure::getKeySeedSuperFeatureNumber()
{
    return _keySeedSuperFeatureNumber;
}

int Configure::getKeySeedGenFeatureThreadNumber()
{
    return _keySeedGenFeatureThreadNumber;
}

std::string
Configure::getRecipeRootPath()
{
    return _RecipeRootPath;
}

std::string Configure::getContainerRootPath()
{
    return _containerRootPath;
}

std::string Configure::getFp2ChunkDBName()
{
    return _fp2ChunkDBName;
}

std::string Configure::getFp2MetaDBame()
{
    return _fp2MetaDBame;
}

uint64_t Configure::getRASessionKeylifeSpan()
{
    return _raSessionKeylifeSpan;
}

int Configure::getSendRecipeBatchSize()
{
    return _sendRecipeBatchSize;
}

std::string Configure::getPOWPriSubscriptionKey()
{
    return _POWPriSubscriptionKey;
}
std::string Configure::getPOWSecSubscriptionKey()
{
    return _POWSecSubscriptionKey;
}

std::string Configure::getKMPriSubscriptionKey()
{
    return _KMPriSubscriptionKey;
}
std::string Configure::getKMSecSubscriptionKey()
{
    return _KMSecSubscriptionKey;
}