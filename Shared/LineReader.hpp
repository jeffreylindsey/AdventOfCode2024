#pragma once

/******************************************************************************
// c_LineReader

Provides a convenient way to iterate through one line at a time from a given
input stream.
-----------------------------------------------------------------------------*/
class c_LineReader
{
	//--- Public Types --------------------------------------------------------
	public:
		class iterator;

	//--- Public Methods ------------------------------------------------------
	public:
		explicit c_LineReader(std::istream& r_Input);

		iterator begin();
		iterator end();

	//--- Private Members -----------------------------------------------------
	private:
		std::istream& m_r_Input;

	//--- Subclasses ----------------------------------------------------------
	public:
		#pragma region iterator
/*****************************************************************************/
// iterator

class iterator
{
	friend class c_LineReader;

	//--- Public Types --------------------------------------------------------
	public:
		using value_type = std::string;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::input_iterator_tag;

	//--- Public Methods ------------------------------------------------------
	public:
		iterator() = default;

		reference operator*();
		pointer operator->();

		iterator& operator++();
		iterator operator++(int);

		bool operator==(const iterator& Right) const;
		bool operator!=(const iterator& Right) const;

	//--- Private Methods -----------------------------------------------------
	private:
		explicit iterator(std::istream& r_Input);

		void ReadNext();

	//--- Private Members -----------------------------------------------------
	private:
		std::istream* m_p_Input = nullptr;

		value_type m_Line = {};
};

/*****************************************************************************/
		#pragma endregion
};

/*****************************************************************************/
