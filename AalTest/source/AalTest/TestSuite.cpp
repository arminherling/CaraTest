#include "TestSuite.h"

namespace AalTest
{
    TestSuite::TestSuite(const QString& name)
        : m_name{ name }
        , m_tests{}
    {
    }
}
