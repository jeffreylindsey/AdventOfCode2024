#include "Precompiled.hpp"
#include "LineReader.hpp"

/*****************************************************************************/
// c_LineReader

/*===========================================================================*/
c_LineReader::c_LineReader(std::istream& r_Input)
	: m_r_Input(r_Input)
{
}

/*===========================================================================*/
c_LineReader::iterator c_LineReader::begin()
{
	return iterator(m_r_Input);
}

/*===========================================================================*/
c_LineReader::iterator c_LineReader::end()
{
	return iterator();
}

/*****************************************************************************/
// c_LineReader::iterator

/*===========================================================================*/
c_LineReader::iterator::reference c_LineReader::iterator::operator*()
{
	return m_Line;
}

/*===========================================================================*/
c_LineReader::iterator::pointer c_LineReader::iterator::operator->()
{
	return &m_Line;
}

/*===========================================================================*/
c_LineReader::iterator& c_LineReader::iterator::operator++()
{
	ReadNext();
	return *this;
}

/*===========================================================================*/
c_LineReader::iterator c_LineReader::iterator::operator++(int)
{
	iterator Next = *this;
	++Next;
	return Next;
}

/*===========================================================================*/
bool c_LineReader::iterator::operator==(const iterator& Right) const
{
	return (m_p_Input == Right.m_p_Input);
}

/*===========================================================================*/
bool c_LineReader::iterator::operator!=(const iterator& Right) const
{
	return !(*this == Right);
}

/*===========================================================================*/
c_LineReader::iterator::iterator(std::istream& r_Input)
	: m_p_Input(&r_Input)
{
	ReadNext();
}

/*===========================================================================*/
void c_LineReader::iterator::ReadNext()
{
	std::getline(*m_p_Input, m_Line);

	if (m_p_Input->eof() || m_p_Input->fail())
		m_p_Input = nullptr;
}

/*===========================================================================*/
