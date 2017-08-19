#pragma once

typedef pthread_key_t	tls_key_t;

bool createTls(tls_key_t* key);
void* getTlsValue(tls_key_t key);
bool setTlsValue(tls_key_t key, void* data);
void deleteTls(tls_key_t key);
