#ifndef POINT_H
#define POINT_H

#include <iostream>


namespace pf
{
	class Point
	{
	public:
		/////////////////////////////////////////////////////////////////
		/// \brief Coordinant.
		/////////////////////////////////////////////////////////////////
		int x, y;

		/////////////////////////////////////////////////////////////////
		/// \brief Default constructor, initializes point (0, 0).
		/////////////////////////////////////////////////////////////////
		Point();

		/////////////////////////////////////////////////////////////////
		/// \brief Constructor with arguments.
		/////////////////////////////////////////////////////////////////
		Point(int, int);

		/////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		/////////////////////////////////////////////////////////////////
		Point(const Point &);

		/////////////////////////////////////////////////////////////////
		/// \brief Point destructor.
		/////////////////////////////////////////////////////////////////
		~Point() {}

		/////////////////////////////////////////////////////////////////
		/// \brief Assignment operator.
		/////////////////////////////////////////////////////////////////
		Point& operator=(const Point &);

		/////////////////////////////////////////////////////////////////
		/// \brief Compund assignment.
		/////////////////////////////////////////////////////////////////
		Point& operator+=(const Point &);

		/////////////////////////////////////////////////////////////////
		/// \brief Addition operator overload.
		/////////////////////////////////////////////////////////////////
		friend Point operator+(Point, const Point &);

		/////////////////////////////////////////////////////////////////
		/// \brief Equal operator overload.
		/////////////////////////////////////////////////////////////////
		friend bool operator== (const Point &, const Point &);

		/////////////////////////////////////////////////////////////////
		/// \brief Different operator overload.
		/////////////////////////////////////////////////////////////////
		friend bool operator!= (const Point &, const Point &);

		/////////////////////////////////////////////////////////////////
		/// \brief Writing into stream operator overload.
		/////////////////////////////////////////////////////////////////
		friend std::ostream& operator<<(std::ostream &, const Point &);
	};
}

#endif // !POINT_H

