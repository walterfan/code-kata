/*
 * TinyChart.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: walter
 */

#include "TinyChart.h"


//---------------------------class TinyChart-------------------//
void TinyChart::SetTitle(char* title)
{
    strncpy(m_szTitle,title,NAMESIZE);
}
void TinyChart::DrawTitle()
{
    if(strlen(m_szTitle)>0)
        gdImageString(m_pImage, gdFontSmall,m_TopLeftPoint.x+5,m_TopLeftPoint.y+5, (unsigned char *)m_szTitle, m_TitleColor);

}
void TinyChart::DrawFrame()
{
    //gdImageFilledRectangle
    m_FrameTopLeftPoint.x=m_TopLeftPoint.x+m_nLeftMargin;
    m_FrameTopLeftPoint.y=m_TopLeftPoint.y+m_nTopMargin;
    m_FrameBottomRightPoint.x=m_BottomRightPoint.x-m_nRightMargin;
    m_FrameBottomRightPoint.y=m_BottomRightPoint.y-m_nBottomMargin;

    gdImageRectangle(m_pImage,m_FrameTopLeftPoint.x,m_FrameTopLeftPoint.y,\
                    m_FrameBottomRightPoint.x,m_FrameBottomRightPoint.y,m_BorderColor);
    gdImageFill(m_pImage, m_FrameTopLeftPoint.x+5, m_FrameTopLeftPoint.y+5, m_FrameColor);
};
void TinyChart::DrawBorder()
{
    gdImageRectangle(m_pImage,m_TopLeftPoint.x,m_TopLeftPoint.y,\
                    m_BottomRightPoint.x,m_BottomRightPoint.y,m_BorderColor);
};

void TinyChart::SetHorziontalScale(STR_VEC& vecXScale)
{
    m_nHorziontalScale=vecXScale.size();
    int nData=vecXScale.size();
    for (int i = 0; i < m_nHorziontalScale; ++i)
    {
        TinyScale* pScale=new TinyScale(m_pImage,m_BorderColor);
        pScale->SetOffset(0,2);
        pScale->SetFont(gdFontTiny);
        if(i<nData)
            pScale->SetText(vecXScale[i].c_str());
        else
            pScale->SetText("");
        m_xScale.push_back(pScale);
    }

}

void TinyChart::DrawGrid()
{
    //cout<<__FILE__<<", "<<__LINE__<<" TinyChart::DrawGrid "\
    <<m_nHorziontalScale<<", "<<m_nVerticalScale<<endl;
    int nFrameWidth=m_FrameBottomRightPoint.x-m_FrameTopLeftPoint.x;
    int nFrameHeight=m_FrameBottomRightPoint.y-m_FrameTopLeftPoint.y;
    if(m_nHorziontalScale==0||m_nVerticalScale==0)
    {
        TRACE(" Error m_nHorziontalScale=" << m_nHorziontalScale  << ", m_nVerticalScale="<< m_nVerticalScale);
        return;
    }
    int nWidthUnit=nFrameWidth/m_nHorziontalScale;
    int nHeightUnit=nFrameHeight/m_nVerticalScale;
    int nXScale=m_xScale.size();
    int nYScale=m_yScale.size();
    //--Draw Vertical lines and horziontal scale
    for (int i = 0; i < m_nHorziontalScale; ++i)
    {
        int x1=m_FrameTopLeftPoint.x+i*nWidthUnit;
        int y1=m_FrameBottomRightPoint.y;
        int x2=x1;
        int y2=m_FrameBottomRightPoint.y-5;
        gdImageLine(m_pImage,x1,y1,x2,y2,m_BorderColor);
        if(nXScale>i)
        {
            m_xScale[i]->SetPosition(x1,y1);
            m_xScale[i]->Draw();
        }

    }
    //--Draw Horziontal lines and vertical scale
    for (int j = 0;  j <=m_nVerticalScale; ++j)
    {
        int x1=m_FrameTopLeftPoint.x;
        int y1=m_FrameTopLeftPoint.y+j*nHeightUnit;
        int x2=m_FrameBottomRightPoint.x;
        int y2=y1;
        if(j <m_nVerticalScale)
            gdImageLine(m_pImage,x1,y1,x2,y2,m_BorderColor);
        if(nYScale>j)
        {
            m_yScale[j]->SetPosition(x1,y1);
            m_yScale[j]->Draw();
        }

    }
}

void TinyChart::Draw()
{
    DrawBorder();
    DrawFrame();
    DrawGrid();
    DrawTitle();
    //SetData();
    Plot();
};

int TinyChart::GetBigerScale(int nScale)
{
    int nMaxScale=nScale,e10;
    if(nMaxScale > 10)
    {
        e10 = 0;
        while(nMaxScale > 10)
        {
            nMaxScale = (int)ceil(nMaxScale / 10)+1;
            e10++;
        }
        nMaxScale = nMaxScale * (int)pow(10, e10);
    }
    else if(nMaxScale > 0 && nMaxScale <= 10)
        nMaxScale= 10;
    else
        nMaxScale= 0;
    return nMaxScale;
};

//-------------------------class TinyColumnChart------------------//
void TinyColumnChart::SetVerticalScale(INT_VEC& vecYScale)
{
    int nData=vecYScale.size();
    if(nData==0)
        return;
    int nSum=0,nAvg=0,nMax=0,nMin=0,nUnit=0;
    for (int j = 0; j < nData; ++j)
    {
        if(j==0)
        {
            nMax=nMin=vecYScale[j];
        }
        else
        {
            if(vecYScale[j]>nMax)
                nMax=vecYScale[j];
            if(vecYScale[j]<nMin)
                nMin=vecYScale[j];

        }
        //nSum=nSum+vecYScale[j];
        m_nData.push_back(vecYScale[j]);
    }
    nAvg=(nMax+nMin)/2;
    char szBuf[NAMESIZE];
    strncpy(szBuf,m_szTitle,NAMESIZE);
    sprintf(m_szTitle,szBuf,nAvg);

    int nMaxScale=GetBigerScale(nMax-nAvg);
    int nUnitScale=2*nMaxScale/nData;

    m_nVerticalScale=nData;

    for (int i = 0; i <= nData; ++i)
    {
        TinyScale* pScale=new TinyScale(m_pImage,m_BorderColor);

        int nScale=nMaxScale-nUnitScale*i;
        //cout<<__FILE__<<", "<<__LINE__<<" nScale: "<<nScale<<endl;
        char szBuf[10];
        sprintf(szBuf,"%d",nScale);

        pScale->SetText(szBuf);
        pScale->SetOffset(-28,0);
        m_yScale.push_back(pScale);
    }

    for (int k = 0; k < nData; ++k)
    {
        m_nData[k]=m_nData[k]-nAvg;
    }
}


void TinyColumnChart::Plot()
{
    TRACE(" TinyColumnChart::SetData");
    int nColumnWidth=10,yMin=10,yZero=0,yMax=0,nMax=0;
    int nXUnit=m_xScale[1]->m_TopLeftPoint.x-m_xScale[0]->m_TopLeftPoint.x;

    if(m_yScale.size()>5&&m_yScale[5]!=NULL)
    {
        yZero=m_yScale[5]->m_TopLeftPoint.y;
        yMax=m_yScale[0]->m_TopLeftPoint.y;
        yMin=m_yScale[m_yScale.size()-1]->m_TopLeftPoint.y;
        nMax=atoi(m_yScale[0]->m_text);
    }
    for (int i = 0; i < m_nData.size(); ++i)
    {
        int x1=m_xScale[i]->m_TopLeftPoint.x+(nXUnit-m_nShapeWidth)/2;
        int y1=yZero;
        int x2=x1+m_nShapeWidth;
        int y2=m_nData[i];

        if(m_nData[i]>0)
            y2=yZero-(yZero-yMax)*m_nData[i]/nMax;
        else
            y2=yZero+(yZero-yMax)*abs(m_nData[i])/nMax;

        TinyRectangle* pRec=new TinyRectangle(m_pImage,x1,y1,x2,y2);
        pRec->SetBgColor(AZURY);
        pRec->Draw();
        pRec->Fill();
        delete pRec;
    }
}
//---------------------------class TinyCurveChart---------------//

void TinyCurveChart::SetVerticalScale(INT_VEC& vecYScale)
{
    int nData=vecYScale.size();
    if(nData==0)
        return;
    int nMax=0;
    for (int j = 0; j < nData; ++j)
    {
        if(vecYScale[j]>nMax)
            nMax=vecYScale[j];
        m_nData.push_back(vecYScale[j]);
    }
    int nMaxScale=GetBigerScale(nMax);
    int nUnitScale=nMaxScale/nData;
    m_nVerticalScale=nData;
    for (int i = 0; i < m_nVerticalScale; ++i)
    {
        TinyScale* pScale=new TinyScale(m_pImage,m_BorderColor);
        int nScale=nMaxScale-nUnitScale*i;

        pScale->SetOffset(-28,-5);
        char szBuf[10];
        sprintf(szBuf,"%d",nScale);
        pScale->SetText(szBuf);
        m_yScale.push_back(pScale);
    }

}

void TinyCurveChart::Plot()
{
    TRACE(" TinyCurveChart::Plot");
    int nYScale=m_yScale.size();
    int nData=m_nData.size();
    int yZero,yMax,nMax;
    int nXUnit=m_xScale[1]->m_TopLeftPoint.x-m_xScale[0]->m_TopLeftPoint.x;
    int nRadius=m_nShapeWidth/2;
    if(nYScale>0)
    {
        yZero=m_yScale[nYScale-1]->m_TopLeftPoint.y;
        yMax=m_yScale[0]->m_TopLeftPoint.y;
        nMax=atoi(m_yScale[0]->m_text);
    }

    for (int i = 0; i < nData; ++i)
    {
        int x=m_xScale[i]->m_TopLeftPoint.x+nXUnit/2;
        int y=yZero-(yZero-yMax)*m_nData[i]/nMax;

        TinyCircle *pCircle=NULL,*pLastCircle=NULL;
        pCircle=new TinyCircle(m_pImage,x,y,nRadius);
        m_Shapes.push_back(pCircle);
        pCircle->SetBorderColor(m_FrColor);
        pCircle->SetBgColor(m_FrColor);
        pCircle->Draw();
        pCircle->Fill();
        if(i>0)
        {
            pLastCircle=(TinyCircle*)m_Shapes[i-1];
            gdImageLine(m_pImage,pLastCircle->GetCentre().x,pLastCircle->GetCentre().y,\
            pCircle->GetCentre().x,pCircle->GetCentre().y,m_FrColor);
        }
    }
}
//---------------------------class TinyHistogram---------------------------//
void TinyHistogram::SetHorziontalScale(STR_VEC& vecXScale)
{
    m_nHorziontalScale=vecXScale.size();
    int nData=vecXScale.size();
    for (int i = 0; i < m_nHorziontalScale; ++i)
    {
        TinyScale* pScale=new TinyScale(m_pImage,m_BorderColor);
        int yOffset=i%2==0?0:10;
        pScale->SetOffset(5,yOffset);
        if(i<nData)
            pScale->SetText(vecXScale[i].c_str());
        else
            pScale->SetText("");
        m_xScale.push_back(pScale);
    }

}
void TinyHistogram::SetVerticalScale(INT_VEC& vecYScale)
{
    m_nVerticalScale=vecYScale.size();

    int nData=vecYScale.size();
    if(nData==0)
        return;
    int nSum=0,nAvg=0,nMax=0,nMin=0,nUnit=0;
    for (int j = 0; j < nData; ++j)
    {
        if(j==0)
        {
            nMax=vecYScale[j];
        }
        else
        {
            if(vecYScale[j]>nMax)
                nMax=vecYScale[j];
        }
        //nSum=nSum+vecYScale[j];
        m_nData.push_back(vecYScale[j]);
    }

    int nMaxScale=GetBigerScale(nMax);
    int nUnitScale=nMaxScale/nData;


    for (int i = 0; i <= nData; ++i)
    {
        TinyScale* pScale=new TinyScale(m_pImage,m_BorderColor);

        int nScale=nMaxScale-nUnitScale*i;
        //int nOffset=
        char szBuf[10];
        sprintf(szBuf,"%d",nScale);

        pScale->SetText(szBuf);
        pScale->SetOffset(2-m_nLeftMargin,-2);
        m_yScale.push_back(pScale);
    }

}

void TinyHistogram::Plot()
{
    TRACE("TinyHistogram::SetData");
    int nXUnit=m_xScale[1]->m_TopLeftPoint.x-m_xScale[0]->m_TopLeftPoint.x;

    int yZero=m_FrameBottomRightPoint.y;
    int yMax=m_FrameTopLeftPoint.y;
    int nMax=atoi(m_yScale[0]->m_text);
    int nSize = m_nData.size();
    for (int i = 0; i < nSize; ++i)
    {
        int x1=m_xScale[i]->m_TopLeftPoint.x+(nXUnit-m_nShapeWidth)/2;
        int y1=yZero;
        int x2=x1+m_nShapeWidth;
        int y2=yZero-(yZero-yMax)*m_nData[i]/nMax;

        TinyRectangle* pRec=new TinyRectangle(m_pImage,x1,y1,x2,y2);
        pRec->SetBgColor(AZURY);
        pRec->Draw();
        pRec->Fill();
        delete pRec;
    }
}
//------------------class TinyPieChart-------------------------//
void TinyPieChart::Draw()
{
    DrawFrame();
    DrawLegend();
    //DrawTitle();
    Plot();
}
TinyColor TinyPieChart::GetRandColor(int index)
{
     switch(index)
     {
        case 0:
            return gdImageColorAllocate(m_pImage,RED);
        case 1:
            return gdImageColorAllocate(m_pImage,GREEN);
        case 2:
            return gdImageColorAllocate(m_pImage,BLUE);
        case 3:
            return gdImageColorAllocate(m_pImage,MAGENTA);
        case 4:
            return gdImageColorAllocate(m_pImage,CYAN);
        case 5:
            return gdImageColorAllocate(m_pImage,YELLOW);
        case 6:
            return gdImageColorAllocate(m_pImage,AZURY);
        default:
            int r,g,b;
            srand(time(NULL));
            r=rand()%255;
            g=rand()%255;
            b=rand()%255;
            return gdImageColorAllocate(m_pImage,r,g,b);
   }
}
void TinyPieChart::DrawFrame()
{
    m_FrameTopLeftPoint.x=m_TopLeftPoint.x+2*m_nLeftMargin+m_nShapeWidth;
    m_FrameTopLeftPoint.y=m_TopLeftPoint.y+m_nTopMargin;
    m_FrameBottomRightPoint.x=m_BottomRightPoint.x-m_nRightMargin;
    m_FrameBottomRightPoint.y=m_BottomRightPoint.y-m_nBottomMargin;

    TinyRectangle rec(m_pImage,m_FrameTopLeftPoint.x,m_FrameTopLeftPoint.y,\
        m_FrameBottomRightPoint.x,m_FrameBottomRightPoint.y);
    rec.Draw();
}
void TinyPieChart::DrawLegend()
{
    int nFrameWidth=m_FrameBottomRightPoint.x-m_FrameTopLeftPoint.x;
    int nFrameHeight=m_FrameBottomRightPoint.y-m_FrameTopLeftPoint.y;
    int nCount=8;
    if(m_nHorziontalScale==0||m_nVerticalScale==0)
    {
        TRACE("Error, m_nHorziontalScale="<<m_nHorziontalScale<<", m_nVerticalScale="<<m_nVerticalScale);
        return;
    }
    int nWidthUnit=nFrameWidth/(m_nVerticalScale/nCount+1);
    int nHeightUnit=nFrameHeight/nCount;

    int nBoxWidth=nWidthUnit/8;
    int nBoxHeight=nHeightUnit/2;


    int x=0;
    int y=0;

    for (int i = 0; i < m_nVerticalScale; ++i)
    {
        if(i==0)
        {
            x=m_FrameTopLeftPoint.x+20;
            y=m_FrameTopLeftPoint.y+20;
        }
        else if(i>0&&i%(nCount-1)==0)
        {
            x=x+nWidthUnit;
            y=m_FrameTopLeftPoint.y+20;
        }
        else
        {
            y=y+2*nBoxHeight;
        }

        TinyRectangle* pBox=new TinyRectangle(m_pImage,x,y,x+nBoxWidth,y+nBoxHeight);
        pBox->SetBgColor(GetRandColor(i));
        pBox->Draw();
        pBox->Fill();
        m_Shapes.push_back(pBox);

        m_xScale[i]->SetPosition(x+nBoxWidth+20,y);
        m_xScale[i]->Draw();

        m_yScale[i]->SetPosition(x+nBoxWidth+120,y);
        m_yScale[i]->Draw();

    }

}
void TinyPieChart::SetHorziontalScale(STR_VEC& vecXScale)
{
    m_nHorziontalScale=vecXScale.size();
    int nData=vecXScale.size();
    for (int i = 0; i < m_nHorziontalScale; ++i)
    {
        TinyScale* pScale=new TinyScale(m_pImage,m_BorderColor);
        pScale->SetOffset(0,2);
        if(i<nData)
            pScale->SetText(vecXScale[i].c_str());
        else
            pScale->SetText("");
        m_xScale.push_back(pScale);
    }

}
/* for pie chart, calculate angle of every datacenter
    vecYScale: vector of attendee number of every dc
    nYscale: count of datacenter
*/
void TinyPieChart::SetVerticalScale(INT_VEC& vecYScale)
{
    m_nVerticalScale=vecYScale.size();
    m_nHorziontalScale=vecYScale.size();
    //int nData=vecYScale.size();
    if(m_nVerticalScale==0)
        return;
    int nSum=0;
    for (int j = 0; j < m_nVerticalScale; ++j)
    {
        nSum=nSum+vecYScale[j];
    }
    if(nSum==0)
        return;
    else
        m_nSum = nSum;

    for (int i = 0; i < m_nVerticalScale; ++i)
    {
        m_nData.push_back(10000*vecYScale[i]/nSum);

        TinyScale* pScale=new TinyScale(m_pImage,m_BorderColor);
        float fScale = 100 * (float) vecYScale[i] / (float)nSum;
        char szBuf[20];
        snprintf(szBuf,20, "%8.2f%%",fScale);
        pScale->SetText(szBuf);
        m_yScale.push_back(pScale);
    }


}

void TinyPieChart::Plot()
{
    TRACE("TinyPieChart::Plot");
    int nRadius=m_nShapeWidth/2;
    TinyPoint centre;
    centre.x=m_nLeftMargin+nRadius;
    centre.y=m_nTopMargin+nRadius;

    TinyCircle circle(m_pImage,centre.x,centre.y,nRadius);
    circle.SetBgColor(GRAY);
    circle.Draw();

    float start_angel=0;
    float end_angel=0;
    int nSize = m_nData.size();
    for (int i = 0; i < nSize; ++i)
    {
        float angel=m_nData[i]*0.036;//==m_nData/10000*360
        end_angel=start_angel+angel;

        TinyEclipse eclipse(m_pImage,centre.x,centre.y,nRadius);
        eclipse.SetSize(nRadius,nRadius);
        eclipse.SetAngel(start_angel,end_angel);
        TRACE(i<<"--"<<m_nData[i]<<"-->"<<start_angel<<", "<<end_angel);
        TinyColor color=m_Shapes[i]->GetBgColor();//GetRandColor(i);
        eclipse.SetBgColor(color);
        eclipse.Draw();
        eclipse.Fill();
        start_angel=end_angel;
    }

}
//------------------class TinyDiagram--------------------------//
TinyDiagram::TinyDiagram(const char* filename,int width,int height)
    :m_nWidth(width),m_nHeight(height), m_szFileName(""),m_szTitle("")
{
    m_pImage=gdImageCreate(width,height);
    SetFileName(filename);
};

TinyDiagram::~TinyDiagram()
{
    if(m_pImage!=NULL)
    {
        gdImageDestroy(m_pImage);
        m_pImage=NULL;
    }
};

int TinyDiagram::SetFileName(const char* filename)
{
    strncpy(m_szFileName,filename,NAMESIZE);
    return 0;
}

int TinyDiagram::SetTitle(const char* title)
{
    strncpy(m_szTitle,title,NAMESIZE);
    return 0;
}

void TinyDiagram::SetLabels(STR_VEC& xScales)
{
    this->m_vecName = xScales;
}

void TinyDiagram::SetValues(INT_VEC& yScales)
{
    this->m_vecData = yScales;
}

void TinyDiagram::DrawBorder()
{
    TinyRectangle rec(m_pImage,0,0,m_nWidth,m_nHeight);
    rec.Draw();
}
void TinyDiagram::WriteImage()
{
    FILE *pngout;
    pngout = fopen(m_szFileName, "wb");
    gdImagePng(m_pImage, pngout);
    fclose(pngout);

};

//------------------class TinyTrendlineDiagram-----------------//
TinyTrendlineDiagram::TinyTrendlineDiagram(const char* filename,int width,int height):TinyDiagram(filename,width,height)
{
    m_pCurveChart=new TinyCurveChart(m_pImage);
    m_pColumnChart=new TinyColumnChart(m_pImage);
}
TinyTrendlineDiagram::~TinyTrendlineDiagram()
{
    if(m_pImage!=NULL)
    {
        gdImageDestroy(m_pImage);
        m_pImage=NULL;
    }
    if(m_pCurveChart!=NULL)
        delete m_pCurveChart;
    if(m_pColumnChart!=NULL)
        delete m_pColumnChart;
};

void TinyTrendlineDiagram::Draw()
{
    DrawBorder();



    TRACE("TinyTrendlineDiagram::Draw ColumnChart"<<0<<", ");
    m_pColumnChart->SetPosition(0,0,m_nWidth/2,m_nHeight);
    m_pColumnChart->SetMargin(30,30,30,10);
    m_pColumnChart->SetTitle(m_szTitle);
    m_pColumnChart->SetHorziontalScale(m_vecName);
    m_pColumnChart->SetVerticalScale(m_vecData);
    m_pColumnChart->Draw();
    TRACE("TinyTrendlineDiagram::Draw CurveChart"<<m_nWidth/2<<", "<<0);
    m_pCurveChart->SetPosition(m_nWidth/2,0,m_nWidth,m_nHeight);
    m_pCurveChart->SetMargin(30,30,30,10);
    m_pCurveChart->SetShapeWidth(8);
    m_pCurveChart->SetHorziontalScale(m_vecName);
    m_pCurveChart->SetVerticalScale(m_vecData);
    m_pCurveChart->Draw();
}
//---------------------------class TinyDatacenterDiagram--------//
TinyDistributionDiagram::TinyDistributionDiagram(const char* filename,int width,int height)
:TinyDiagram(filename,width,height)
{
    m_pPieChart=new TinyPieChart(m_pImage);
    m_pHistogram=new TinyHistogram(m_pImage);
}
TinyDistributionDiagram::~TinyDistributionDiagram()
{
    if(m_pImage!=NULL)
    {
        gdImageDestroy(m_pImage);
        m_pImage=NULL;
    }
    if(m_pPieChart!=NULL)
        delete m_pPieChart;
    if(m_pHistogram!=NULL)
        delete m_pHistogram;
}
void TinyDistributionDiagram::Draw()
{
    DrawBorder();

    TRACE("TinyDatacenterDiagram::Draw TinyPieChart");
    m_pPieChart->SetPosition(0,0,m_nWidth,m_nHeight/2);
    m_pPieChart->SetMargin(30,30,40,40);
    m_pPieChart->SetShapeWidth(200);//radius of the pie
    m_pPieChart->SetHorziontalScale(m_vecName);
    m_pPieChart->SetVerticalScale(m_vecData);
    m_pPieChart->Draw();
    TRACE("TinyDatacenterDiagram::Draw TinyHistogram");
    m_pHistogram->SetPosition(0,m_nHeight/2,m_nWidth,m_nHeight);
    m_pHistogram->SetMargin(30,30,40,40);
    m_pHistogram->SetTitle(m_szTitle);
    m_pHistogram->SetShapeWidth(20);//note: the m_nShapeWidth should be resize by xScale count
    m_pHistogram->SetHorziontalScale(m_vecName);
    m_pHistogram->SetVerticalScale(m_vecData);
    m_pHistogram->Draw();
}

