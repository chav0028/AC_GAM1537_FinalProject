#include "Derived.h"
#include "Cshape.h"  
#include <vector>
#include <gdiplus.h>
#include <Windows.h>

//Math required for calculating lines
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Gdiplus;
using namespace std;

//Note that because of how the code is made, when it referts in comments to top right corner, top left corner, etc. It refers to normal situations, as the default values. This may changes
//according to the values passed to the constructor. This titles are used only for orienting when creating the function, the function works regardless of what value
//gets passed in, some really rare special exceptions excluded

//Line
CLine::CLine(int x, int y, int len, float angle, float t, COLORREF col) :CShape(x, y, 4, t, col)
{
    //Set starting points for the line
    m_x = x;
    m_y = y;

    //Convert the angle from degrees to radians
    angle = (angle*M_PI) / 180;

    //Calculate the ending points for the line using the angle and length
    //Angle=direction
    //Len=magnitude
    m_endX = (cosf(angle))*len;//Cos = x
    m_endX += m_x;
    m_endY = (sinf(angle))*len;//Sin = y
    m_endY += m_y;
}

//Functions to get the ending position of lines
int CLine::getEndX()
{
    return m_endX;
}

int CLine::getEndY()
{
    return m_endY;
}

/**************************************************
CLine::draw
Description: Draws a straight line, according to the starting
and ending positions calculated in the constructor.
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/03/23
Author: Alvaro Chavez Mixco
***************************************************/
void CLine::draw(Graphics &graphics)
{
    //GDI+ function to draw line, takes the pointer to the pen, starting x and y positions and the ending x and y positions of the line
    graphics.DrawLine(m_pen, m_x, m_y, m_endX, m_endY);
}



//Parallelogram
CParallelogram::CParallelogram(int x, int y, int w, int h, float topLeftAngleDeg, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CShape(x, y, 4, t, colStroke, alphaFill, colFill)
{
    //Parallelogram inner angles add 360
    //Angle 1 and angle 2 are supplementary.
    //So if top left angle==angle1, angle1+angle2=180;
    //Top left angle==top right angle & bottom left angle==bottom right angle

    //Set values
    m_width = w;
    m_height = h;
    m_angle1 = topLeftAngleDeg;
    m_angle2 = 180 - topLeftAngleDeg;//Supplementary angles

    //Cant use lines since I don't have the magnitude (hypotenuse)
    //Use an imaginary right triangle on one edge of the parallelogram to find measures
    /*  ____________
       / |         /
       M/  |H       /
       /___|_______/
       */
    //Cos(angle)=adj/hyp == hyp=adj/cos(angle).Use cos to find the hypotenuse, height=adjacent
    float angleRadians = ((m_angle1 - 90)*M_PI) / 180;//Temporary angle used to calculate magnitude, not done to m_angle1 because line already converts it to radian    
    //Angle -90 degrees to get just the inner angles of the imaginary right triangle
    int magnitude = m_height / (cosf(angleRadians));

    //Use lines to get the points of the parallelogram
    CLine leftSide(m_x, m_y, magnitude, m_angle1, m_thick, m_colorStroke);
    //Bottom left vertex
    int x2 = leftSide.getEndX();
    int y2 = leftSide.getEndY();
    //bottom right vertex
    int x3 = x2 + m_width;
    int y3 = y2;
    //top right vertex
    int x4 = m_x + m_width;
    int y4 = m_y;

    //Add points
    m_points.push_back(Point(m_x, m_y));//Top left
    m_points.push_back(Point(x2, y2));//Bottom left
    m_points.push_back(Point(x3, y3));//Bototm Right
    m_points.push_back(Point(x4, y4));//Top Right
}

/**************************************************
CParallelogram::draw
Description: Draws a parallelogram using GDI+, it uses a vector
of points calculated in the class constructor.x and y is not
guaranteed to be top left, depends on the angle of it.(if (angle<=90)(X,y)==top left)
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/03/22
Author: Alvaro Chavez Mixco
***************************************************/
void CParallelogram::draw(Gdiplus::Graphics &graphics)
{
    m_numPoints = m_points.size();//Get number of points
    graphics.DrawPolygon(m_pen, m_points.data(), m_numPoints);//Draw the figure
    if (m_alphaFill != 0)//if the image will actually be filled.
    {
        graphics.FillPolygon(m_brush, m_points.data(), m_numPoints);//Fill the figure
    }
}



//Rectangle
CRectangle::CRectangle(int x, int y, int w, int h, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CParallelogram(x, y, w, h, 90, t, colStroke, alphaFill, colFill)
{
    //Only passes certain values to parent class.
    //The angle of 90 is hard coded in constructor because that is what makes a rectangle from a parallelogram
}

/**************************************************
CRectangle::draw
Description: Draws a rectangle using GDI+ according
to the characteristics passed by the user in the constructor
(position (X,y), width,height, pen thickness, color of
stroke, alpha transparency, color of fill),
x and y are the top left.
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/03/19
Author: Alvaro Chavez Mixco
***************************************************/
void CRectangle::draw(Graphics &graphics)
{
    //Since a rectangle is a parallelogram it could also use the parallelogram draw method, but I believe this method is lighter since it doesn't have to
    //go through each point in the vector
    graphics.DrawRectangle(m_pen, m_x, m_y, m_width, m_height);//x,y, are top left
    if (m_alphaFill != 0)//if the image will actually be filled.
    {
        graphics.FillRectangle(m_brush, m_x, m_y, m_width, m_height);
    }
}



//Square
CSquare::CSquare(int x, int y, int l, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CRectangle(x, y, l, l, t, colStroke, alphaFill, colFill)
{
    //Only passes certain values to parent class.
    //The with and the height are the same length, so the same value is passed for both
}

/**************************************************
CSquare::draw
Description: Draws a square using GDI+, it calls it parent
class (CRectangle) draw function.
x and y are top left.
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/03/22
Author: Alvaro Chavez Mixco
***************************************************/
void CSquare::draw(Graphics &graphics)
{
    CRectangle::draw(graphics);//Call parent class draw function, 
}



//Isosceles trapezoid
CTrapezoid::CTrapezoid(int x, int y, int base, int h, int top, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CParallelogram(x, y, base, h, 120, t, colStroke, alphaFill, colFill)
{
    //The trapezoid could theoretically stand itself as it own class (because it is an isosceles trapezoid that doesn't offer as much customization)
    //However, by being a child class of parallelogram I can use it's draw method, and the variables it has already declared.
    //120 angle (CParallelogram constructor call) is just a fake angle that isn't used

    //Since the angle is not used when drawing the trapezoid, we pass in a fake "angle". So we have to clear the list of points given to us by the parent class constructor
    m_points.clear();

    //Isosceles trapezoid, suppose that the angle of offset is the same (the base and the top are centered)
    //Bottom left corner
    int x1 = m_x; // M_width == base of trapezoid
    int y1 = m_y + m_height;

    //Top left corner
    int x2 = m_x + ((m_width / 2) - (top / 2));//Calculate the x position by going to the middle of the base and then moving to the end of the top.
    int y2 = m_y;

    //Top Right corner
    int x3 = x2 + top;
    int y3 = m_y;

    //Bottom Left corner
    int x4 = m_x + m_width;
    int y4 = y1;

    //Add points
    m_points.push_back(Point(x1, y1));
    m_points.push_back(Point(x2, y2));
    m_points.push_back(Point(x3, y3));
    m_points.push_back(Point(x4, y4));
}

/**************************************************
CTrapezoid::draw
Description: Draws a trapezoid using GDI+, it uses the parent
class (CParallelogram) draw method.It should be noted that it uses the list of points
obtained in the CTrapezoid constructor and NOT the ones used in the CParallelogram constructor.
x and y are not always top left.
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/03/22
Author: Alvaro Chavez Mixco
***************************************************/
void CTrapezoid::draw(Graphics &graphics)
{
    CParallelogram::draw(graphics);
}



//Cube
CCube::CCube(int x, int y, int l, int d, float angle, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CSquare(x, y, l, t, colStroke, alphaFill, colFill)
{
    m_depth = d;
    m_inclinationAngle = angle;
}

/**************************************************
CCube::draw
Description:
Relatively heavy function, since it  fills and calculates the point for each surface
individually.
Draws 2 rectangles with lines between them
to simulate a 3D cube. The function creates the 4 lines objects
and gets the coordinates for the position of the second rectangle.

Return: Void
Date: 2015/03/23
Author: Alvaro Chavez Mixco
***************************************************/
void CCube::draw(Graphics &graphics)
{
    //Create top left rectangle
    graphics.DrawRectangle(m_pen, m_x, m_y, m_width, m_height);

    //Points calculated in here instead of constructor because lines use member variables of the class. So they can't be declared in header,
    // and they have to be drawn
    //Create lines between rectangles
    CLine lineTopLeft(m_x, m_y, m_depth, m_inclinationAngle, m_thick, m_colorStroke);
    CLine lineTopRight(m_x + m_width, m_y, m_depth, m_inclinationAngle, m_thick, m_colorStroke);
    CLine lineBottomLeft(m_x, m_y + m_height, m_depth, m_inclinationAngle, m_thick, m_colorStroke);
    CLine lineBottomRight(m_x + m_width, m_y + m_height, m_depth, m_inclinationAngle, m_thick, m_colorStroke);

    //Draw the lines
    lineTopLeft.draw(graphics);
    lineTopRight.draw(graphics);
    lineBottomLeft.draw(graphics);
    lineBottomRight.draw(graphics);

    //Get the coordinates where the other rectangle will be drawn
    m_secondRectangleX = lineTopLeft.getEndX();
    m_secondRectangleY = lineTopLeft.getEndY();

    //Draw second rectangle
    graphics.DrawRectangle(m_pen, m_secondRectangleX, m_secondRectangleY, m_width, m_height);

    if (m_alphaFill != 0)//if the image will actually be filled. 
    {
        //Fill the rectangles
        graphics.FillRectangle(m_brush, m_x, m_y, m_width, m_height);
        graphics.FillRectangle(m_brush, m_secondRectangleX, m_secondRectangleY, m_width, m_height);

        //Ignore the points obtained in the parallelogram class
        m_points.clear();

        //Get the points to be used to fill the "lines" between the rectangles
        //Add the position to the vector by casting the int coordinates to points (used by GDI+)

        //Paint every surface, one by one; since the direction (angle) of the square can change and therefore mess the fill if all are done at once
        //Left surface
        m_points.push_back(Point(m_x, m_y));//Top Left
        m_points.push_back(Point(m_x, m_y + m_height));//Bottom left
        m_points.push_back(Point(m_secondRectangleX, m_secondRectangleY + m_height));//Bottom Right
        m_points.push_back(Point(m_secondRectangleX, m_secondRectangleY));//Top Right  

        //Delete each point, program appears to give problems (order of points) when painting all surfaces at once
        m_numPoints = m_points.size();//Get how many points are in the array   
        graphics.FillPolygon(m_brush, m_points.data(), m_numPoints);//Fill the space between the rectangles
        m_points.clear();//Erase all elements in vector, to paint new surface

        //Top Surface
        m_points.push_back(Point(m_x, m_y));//Top left
        m_points.push_back(Point(m_secondRectangleX, m_secondRectangleY));//Bottom left
        m_points.push_back(Point(m_secondRectangleX + m_width, m_secondRectangleY));//Bototm Right
        m_points.push_back(Point(m_x + m_width, m_y));//Top Right

        m_numPoints = m_points.size();//Get how many points are in the array   
        graphics.FillPolygon(m_brush, m_points.data(), m_numPoints);//Fill the space between the rectangles
        m_points.clear();//Erase all elements in vector, to paint new surface

        //Right surface
        m_points.push_back(Point(m_x + m_width, m_y));//Top Left
        m_points.push_back(Point(m_x + m_width, m_y + m_height));//Bottom left
        m_points.push_back(Point(m_secondRectangleX + m_width, m_secondRectangleY + m_height));//Bottom Right
        m_points.push_back(Point(m_secondRectangleX + m_width, m_secondRectangleY));//Top left

        m_numPoints = m_points.size();//Get how many points are in the array   
        graphics.FillPolygon(m_brush, m_points.data(), m_numPoints);//Fill the space between the rectangles
        m_points.clear();//Erase all elements in vector, to paint new surface

        //Bottom Surface 
        m_points.push_back(Point(m_x, m_y + m_height));//Top left
        m_points.push_back(Point(m_secondRectangleX, m_secondRectangleY + m_height));//Bottom left
        m_points.push_back(Point(m_secondRectangleX + m_width, m_secondRectangleY + m_height));//Bototm Right
        m_points.push_back(Point(m_x + m_width, m_y + m_height));//Top Right 

        m_numPoints = m_points.size();//Get how many points are in the array   
        graphics.FillPolygon(m_brush, m_points.data(), m_numPoints);//Fill the space between the rectangles
        m_points.clear();//Clear the points for when the figure is drawn again
    }
}



//Triangle-Constructors header file made to explain how it works
/**************************************************
CTriangle::Ctriangle-Constructor
Description: Functions that calculates the position of 2
points of a triangle , and automatically calculates the third
point by "closing" the triangle. It only works when the user
doesn't set both his points in a straight 180 angle degree, but
it offers more customization when building triangles. The constructor
basically allows you to make 2 line, then it automatically closes the 2 lines
to make a triangle
Arguments: int x1, int y1-Position of the first point of the triangl;e
int lSide1-The length of the first side of the triangle (connects (x1,y1) to (x2,y2))
int angle1-The direction in which the first side is.
int lSide2-The length of the second side of the triangle (connects (x2,y2) to (x3,y3))
int angle2-the direction of the second side of the triangle
float t,COLORREF colStroke, int alphaFill, COLORREF colFill-Values used to draw the triangle.
Return: Void
Date: 2015/03/21
Author: Alvaro Chavez Mixco
***************************************************/
CTriangle::CTriangle(int x1, int y1, int lSide1, int angle1, int lSide2, int angle2, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CShape(x1, y1, 3, t, colStroke, alphaFill, colFill)
{
    //Create lines, just used to calculate space between them
    CLine side1(x1, y1, lSide1, angle1);//First side
    //Get the starting position for the second side
    int x2 = side1.getEndX();
    int y2 = side1.getEndY();
    CLine side2(x2, y2, lSide2, angle2);
    //Get starting position for third side
    int x3 = side2.getEndX();
    int y3 = side2.getEndY();

    //Add points
    m_points.push_back(Point(x1, y1));
    m_points.push_back(Point(x2, y2));
    m_points.push_back(Point(x3, y3));
    //GDI+ automatically closes the polygon, no need to go back to beginning point
}

/**************************************************
CTriangle::draw
Description: Draws a triangle using GDI+ according to the
points calculated in the constructor
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/0321
Author: Alvaro Chavez Mixco
***************************************************/
void CTriangle::draw(Graphics &graphics)
{
    m_numPoints = m_points.size();//Calculate how many points there will be
    graphics.DrawPolygon(m_pen, m_points.data(), m_numPoints);
    if (m_alphaFill != 0)//if the image will actually be filled.
    {
        graphics.FillPolygon(m_brush, m_points.data(), m_numPoints);
    }
}



//Isosceles Triangle
CIsoTriangle::CIsoTriangle(int x, int y, int equalSidesLength, int angle, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CTriangle(x, y, equalSidesLength, angle, equalSidesLength, -angle, t, colStroke, alphaFill, colFill)
{
    //Empty, the triangle constructor does everything. This constructor only ensures that two
    //sides of the triangle and 2 of its angles have the same measures.
    //Note that one of the angles is converted to negative, so that it closes the triangle
}

/**************************************************
CIsoTriangle::draw
Description: Draws a isosceles triangle using GDI+ by calling
the parent class (Ctriangle) draw function and the values
established in the constructor.
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/0321
Author: Alvaro Chavez Mixco
***************************************************/
void CIsoTriangle::draw(Graphics &graphics)
{
    CTriangle::draw(graphics);//Call the draw function from parent class
}



//Equilateral triangle
CEquiTriangle::CEquiTriangle(int x, int y, int l, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CTriangle(x, y, l, 60, l, -60, t, colStroke, alphaFill, colFill)
{
    //Empty, the triangle constructor does everything. This constructor only ensures that all the sides
    //have the same measurement and all the angles are 60 degrees 
}

/**************************************************
CEquiTriangle::draw
Description: Draws a isosceles triangle using GDI+ by calling
the parent class (Ctriangle) draw function and the values
established in the constructor.
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/0321
Author: Alvaro Chavez Mixco
***************************************************/
void CEquiTriangle::draw(Graphics &graphics)
{
    CTriangle::draw(graphics);//Call the draw function from parent class
}



//Ellipse
CEllipse::CEllipse(int x, int y, int w, int h, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CShape(x, y, 2, t, colStroke, alphaFill, colFill)
{
    m_width = w;
    m_height = h;
}

/**************************************************
CEllipse::draw
Description: Draws a ellipse using GDI+, and filled it.
It uses the values passed in the class constructor.
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/0321
Author: Alvaro Chavez Mixco
***************************************************/
void CEllipse::draw(Gdiplus::Graphics &graphics)
{
    graphics.DrawEllipse(m_pen, m_x, m_y, m_width, m_height);
    if (m_alphaFill != 0)//if the image will actually be filled.
    {
        graphics.FillEllipse(m_brush, m_x, m_y, m_width, m_height);
    }
}



//Circle
CCircle::CCircle(int x, int y, int diameter, float t, COLORREF colStroke, int alphaFill, COLORREF colFill) :CEllipse(x, y, diameter, diameter, t, colStroke, alphaFill, colFill)
{
    //Width and height are the same measure
    //Constructor only passes the correct values to the parent class CEllipse constructor
}

/**************************************************
CEquiTriangle::draw
Description: Draws a circle using GDI+ by calling
the parent class (CEllipse) draw function and the values
established in the constructor.
Arguments: Graphics (passed by reference) object used
to draw.
Return: Void
Date: 2015/0321
Author: Alvaro Chavez Mixco
***************************************************/
void CCircle::draw(Gdiplus::Graphics &graphics)
{
    CEllipse::draw(graphics);
}