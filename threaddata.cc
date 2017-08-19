#include <pthread.h>

#include "threaddata.h"

bool createTls(tls_key_t* key)
{
	pthread_key_create(key, NULL);
	return true;
}

void* getTlsValue(tls_key_t key)
{
	return pthread_getspecific(key);
}

bool setTlsValue(tls_key_t key, void* data)
{
	return (0 == pthread_setspecific(key, data));
}

void deleteTls(tls_key_t key)
{
	pthread_key_delete(key);
}
