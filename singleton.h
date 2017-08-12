#pragma once

#define DECLARE_SINGLETON( classType ) \
public: \
	static classType& getInstance( void ) { return m_instance; } \
	static classType* getInstancePtr( void ) { return &m_instance; } \
private: \
	static classType	m_instance;

#define DEFINE_SINGLETON( classType )		classType classType::m_instance;
