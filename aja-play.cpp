#include "pch.h"
#include <DShow.h>
#include <atlbase.h>
#include <initguid.h>
#include <dvdmedia.h>
#include <errors.h>
#include <string>
#include <comutil.h>
#include "aja/AJAMultiRenderer.h"
#include "aja/AJARenderer.h"

#pragma comment (lib, "quartz.lib" ) // link with "quartz.lib" (or debug version: "quartzd.lib")
#pragma comment (lib, "strmiids.lib" ) // link with "strmiids.lib" (or debug version: "strmiidsd.lib")
#pragma comment (lib, "comsuppw.lib" ) // link with "comsuppw.lib" (or debug version: "comsuppwd.lib")

BOOL hrcheck(HRESULT hr, const WCHAR* errtext)
{
    if (hr >= S_OK)
        return FALSE;
    TCHAR szErr[MAX_ERROR_TEXT_LEN];
    DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);
    if (res)
        _tprintf(_T("Error %x: %s\n%s\n"), hr, errtext, szErr);
    else
        _tprintf(_T("Error %x: %s\n"), hr, errtext);
    return TRUE;
}

//change this macro to fit your style of error handling
#define CHECK_HR(hr, msg) if (hrcheck(hr, msg)) return hr;
CComPtr<IBaseFilter> CreateFilterByName(const WCHAR* filterName, const GUID& category)
{
    HRESULT hr = S_OK;
    CComPtr<ICreateDevEnum> pSysDevEnum;
    hr = pSysDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
    if (hrcheck(hr, _T("Can't create System Device Enumerator")))
        return NULL;

    CComPtr<IEnumMoniker> pEnumCat;
    hr = pSysDevEnum->CreateClassEnumerator(category, &pEnumCat, 0);

    if (hr == S_OK)
    {
        CComPtr<IMoniker> pMoniker;
        ULONG cFetched;
        while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
        {
            CComPtr<IPropertyBag> pPropBag;
            hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);
            if (SUCCEEDED(hr))
            {
                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                if (SUCCEEDED(hr))
                {
                    if (wcscmp(filterName, varName.bstrVal) == 0) {
                        CComPtr<IBaseFilter> pFilter;
                        hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pFilter);
                        if (hrcheck(hr, _T("Can't bind moniker to filter object")))
                            return NULL;
                        return pFilter;
                    }
                }
                VariantClear(&varName);
            }
            pMoniker.Release();
        }
    }
    return NULL;
}

CComPtr<IPin> GetPin(IBaseFilter* pFilter, LPCOLESTR pinname)
{
    CComPtr<IEnumPins>  pEnum;
    CComPtr<IPin>       pPin;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (hrcheck(hr, _T("Can't enumerate pins.")))
        return NULL;

    while (pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_INFO pinfo;
        pPin->QueryPinInfo(&pinfo);
        BOOL found = !_wcsicmp(pinname, pinfo.achName);
        if (pinfo.pFilter) pinfo.pFilter->Release();
        if (found)
            return pPin;
        pPin.Release();
    }
    printf("Pin not found!\n");
    return NULL;
}


// {89BB1180-565C-49B6-9DED-D6D2DFCA06A8}
DEFINE_GUID(CLSID_AJAAVRenderKona3G0,
    0x89BB1180, 0x565C, 0x49B6, 0x9D, 0xED, 0xD6, 0xD2, 0xDF, 0xCA, 0x06, 0xA8); //AJARenderer64.dll

// {89BB1181-565C-49B6-9DED-D6D2DFCA06A8}
DEFINE_GUID(CLSID_AJAAVRenderKona3G1,
    0x89BB1181, 0x565C, 0x49B6, 0x9D, 0xED, 0xD6, 0xD2, 0xDF, 0xCA, 0x06, 0xA8); //AJARenderer64.dll

// {89BB1182-565C-49B6-9DED-D6D2DFCA06A8}
DEFINE_GUID(CLSID_AJAAVRenderKona3G2,
    0x89BB1182, 0x565C, 0x49B6, 0x9D, 0xED, 0xD6, 0xD2, 0xDF, 0xCA, 0x06, 0xA8); //AJARenderer64.dll

// {89BB1183-565C-49B6-9DED-D6D2DFCA06A8}
DEFINE_GUID(CLSID_AJAAVRenderKona3G3,
    0x89BB1183, 0x565C, 0x49B6, 0x9D, 0xED, 0xD6, 0xD2, 0xDF, 0xCA, 0x06, 0xA8); //AJARenderer64.dll


// {75FEDC00-D525-4181-BF11-9C21CD909416}
DEFINE_GUID(CLSID_AJAMRendererKona3G00,
    0x75FEDC00, 0xD525, 0x4181, 0xBF, 0x11, 0x9C, 0x21, 0xCD, 0x90, 0x94, 0x16); //AJAMultiRenderer64.dll

// {75FEDC01-D525-4181-BF11-9C21CD909416}
DEFINE_GUID(CLSID_AJAMRendererKona3G01,
    0x75FEDC01, 0xD525, 0x4181, 0xBF, 0x11, 0x9C, 0x21, 0xCD, 0x90, 0x94, 0x16); //AJAMultiRenderer64.dll

// {75FEDC10-D525-4181-BF11-9C21CD909416}
DEFINE_GUID(CLSID_AJAMRendererKona3G10,
    0x75FEDC10, 0xD525, 0x4181, 0xBF, 0x11, 0x9C, 0x21, 0xCD, 0x90, 0x94, 0x16); //AJAMultiRenderer64.dll

// {75FEDC11-D525-4181-BF11-9C21CD909416}
DEFINE_GUID(CLSID_AJAMRendererKona3G11,
    0x75FEDC11, 0xD525, 0x4181, 0xBF, 0x11, 0x9C, 0x21, 0xCD, 0x90, 0x94, 0x16); //AJAMultiRenderer64.dll

// {75FEDC20-D525-4181-BF11-9C21CD909416}
DEFINE_GUID(CLSID_AJAMRendererKona3G20,
    0x75FEDC20, 0xD525, 0x4181, 0xBF, 0x11, 0x9C, 0x21, 0xCD, 0x90, 0x94, 0x16); //AJAMultiRenderer64.dll

// {75FEDC21-D525-4181-BF11-9C21CD909416}
DEFINE_GUID(CLSID_AJAMRendererKona3G21,
    0x75FEDC21, 0xD525, 0x4181, 0xBF, 0x11, 0x9C, 0x21, 0xCD, 0x90, 0x94, 0x16); //AJAMultiRenderer64.dll

// {75FEDC30-D525-4181-BF11-9C21CD909416}
DEFINE_GUID(CLSID_AJAMRendererKona3G30,
    0x75FEDC30, 0xD525, 0x4181, 0xBF, 0x11, 0x9C, 0x21, 0xCD, 0x90, 0x94, 0x16); //AJAMultiRenderer64.dll

// {75FEDC31-D525-4181-BF11-9C21CD909416}
DEFINE_GUID(CLSID_AJAMRendererKona3G31,
    0x75FEDC31, 0xD525, 0x4181, 0xBF, 0x11, 0x9C, 0x21, 0xCD, 0x90, 0x94, 0x16); //AJAMultiRenderer64.dll



// {B98D13E7-55DB-4385-A33D-09FD1BA26338}
DEFINE_GUID(CLSID_LAVSplitterSource,
    0xB98D13E7, 0x55DB, 0x4385, 0xA3, 0x3D, 0x09, 0xFD, 0x1B, 0xA2, 0x63, 0x38); //LAVSplitter.ax


// {E8E73B6B-4CB3-44A4-BE99-4F7BCB96E491}
DEFINE_GUID(CLSID_LAVAudioDecoder,
    0xE8E73B6B, 0x4CB3, 0x44A4, 0xBE, 0x99, 0x4F, 0x7B, 0xCB, 0x96, 0xE4, 0x91); //LAVAudio.ax


// {EE30215D-164F-4A92-A4EB-9D4C13390F9F}
DEFINE_GUID(CLSID_LAVVideoDecoder,
    0xEE30215D, 0x164F, 0x4A92, 0xA4, 0xEB, 0x9D, 0x4C, 0x13, 0x39, 0x0F, 0x9F); //LAVVideo.ax


// {CC7BFB42-F175-11D1-A392-00E0291F3959}
DEFINE_GUID(CLSID_VideoEffects1,
    0xCC7BFB42, 0xF175, 0x11D1, 0xA3, 0x92, 0x00, 0xE0, 0x29, 0x1F, 0x39, 0x59); // Scale




HRESULT BuildGraph(IGraphBuilder* pGraph, int outType, int outCard, int outDevice, int outPort, LPCOLESTR srcFile1)
{
    HRESULT hr = S_OK;

    //graph builder
    CComPtr<ICaptureGraphBuilder2> pBuilder;
    hr = pBuilder.CoCreateInstance(CLSID_CaptureGraphBuilder2);
    CHECK_HR(hr, _T("Can't create Capture Graph Builder"));
    hr = pBuilder->SetFiltergraph(pGraph);
    CHECK_HR(hr, _T("Can't SetFiltergraph"));

    //add LAV Splitter Source
    CComPtr<IBaseFilter> pLAVSplitterSource;
    hr = pLAVSplitterSource.CoCreateInstance(CLSID_LAVSplitterSource);
    CHECK_HR(hr, _T("Can't create LAV Splitter Source"));
    hr = pGraph->AddFilter(pLAVSplitterSource, L"LAV Splitter Source");
    CHECK_HR(hr, _T("Can't add LAV Splitter Source to graph"));
    //set source filename
    CComQIPtr<IFileSourceFilter, &IID_IFileSourceFilter> pLAVSplitterSource_src(pLAVSplitterSource);
    if (!pLAVSplitterSource_src)
        CHECK_HR(E_NOINTERFACE, _T("Can't get IFileSourceFilter"));
    hr = pLAVSplitterSource_src->Load(srcFile1, NULL);
    CHECK_HR(hr, _T("Can't load file"));


    //add LAV Audio Decoder
    CComPtr<IBaseFilter> pLAVAudioDecoder;
    hr = pLAVAudioDecoder.CoCreateInstance(CLSID_LAVAudioDecoder);
    CHECK_HR(hr, _T("Can't create LAV Audio Decoder"));
    hr = pGraph->AddFilter(pLAVAudioDecoder, L"LAV Audio Decoder");
    CHECK_HR(hr, _T("Can't add LAV Audio Decoder to graph"));


    //add LAV Video Decoder
    CComPtr<IBaseFilter> pLAVVideoDecoder;
    hr = pLAVVideoDecoder.CoCreateInstance(CLSID_LAVVideoDecoder);
    CHECK_HR(hr, _T("Can't create LAV Video Decoder"));
    hr = pGraph->AddFilter(pLAVVideoDecoder, L"LAV Video Decoder");
    CHECK_HR(hr, _T("Can't add LAV Video Decoder to graph"));


    //connect LAV Splitter Source and LAV Video Decoder
    hr = pGraph->ConnectDirect(GetPin(pLAVSplitterSource, L"Video"), GetPin(pLAVVideoDecoder, L"Input"), NULL);
    CHECK_HR(hr, _T("Can't connect LAV Splitter Source and LAV Video Decoder"));


    //connect LAV Splitter Source and LAV Audio Decoder
    hr = pGraph->ConnectDirect(GetPin(pLAVSplitterSource, L"Audio"), GetPin(pLAVAudioDecoder, L"Input"), NULL);
    CHECK_HR(hr, _T("Can't connect LAV Splitter Source and LAV Audio Decoder"));


    //add AJA M-Renderer Kona3G
    CComPtr<IBaseFilter> pAJARendererKona3G;

    if (outType == 0) {
        switch (outDevice) {
        case 0:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAAVRenderKona3G0);
            break;
        case 1:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAAVRenderKona3G1);
            break;
        case 2:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAAVRenderKona3G2);
            break;
        case 3:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAAVRenderKona3G3);
            break;
        default:
            printf("Output %d is not valid.\n", outDevice);
            return E_INVALIDARG;
        }

        CHECK_HR(hr, _T("Can't create AJA Renderer Kona3G"));
    } else {
        outCard *= 2;
        outCard += outDevice;
        switch (outCard) {
        case 0:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAMRendererKona3G00);
            break;
        case 1:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAMRendererKona3G01);
            break;
        case 2:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAMRendererKona3G10);
            break;
        case 3:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAMRendererKona3G11);
            break;
        case 4:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAMRendererKona3G20);
            break;
        case 5:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAMRendererKona3G21);
            break;
        case 6:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAMRendererKona3G30);
            break;
        case 7:
            hr = pAJARendererKona3G.CoCreateInstance(CLSID_AJAMRendererKona3G31);
            break;
        default:
            printf("Output %d is not valid.\n", outDevice);
            return E_INVALIDARG;
        }

        CHECK_HR(hr, _T("Can't create AJA Renderer Kona3G"));

        CComPtr<IAJAMultiRender> pAJAMultiRender;
        hr = pAJARendererKona3G->QueryInterface(IID_PPV_ARGS(&pAJAMultiRender));
        CHECK_HR(hr, _T("Can't create AJA Renderer Kona3G Control Interface"));
        
        switch (outPort) {
            case 0:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_HDMI);
                break;
            case 1:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_ANALOG);
                break;
            case 2:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_SDI1);
                break;
            case 3:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_SDI2);
                break;
            case 4:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_SDI3);
                break;
            case 5:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_SDI4);
                break;
            case 6:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_SDI5);
                break;
            case 7:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_SDI6);
                break;
            case 8:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_SDI7);
                break;
            case 9:
                pAJAMultiRender->SetVideoOutput(DS_OUTPUTDESTINATION_SDI8);
                break;
        }
    }

    hr = pGraph->AddFilter(pAJARendererKona3G, L"AJA Renderer Kona3G");
    CHECK_HR(hr, _T("Can't add AJA Renderer Kona3G to graph"));


    //connect LAV Audio Decoder and AJA M-Renderer Kona3G - 0:0
    hr = pGraph->ConnectDirect(GetPin(pLAVAudioDecoder, L"Output"), GetPin(pAJARendererKona3G, L"Aud 1"), NULL);
    CHECK_HR(hr, _T("Can't connect LAV Audio Decoder and AJA M-Renderer Kona3G"));


    //add Scale
    CComPtr<IBaseFilter> pScale = CreateFilterByName(L"Scale", CLSID_VideoEffects1);
    hr = pGraph->AddFilter(pScale, L"Scale");
    CHECK_HR(hr, _T("Can't add Scale to graph"));


    //connect LAV Video Decoder and Scale
    hr = pGraph->ConnectDirect(GetPin(pLAVVideoDecoder, L"Output"), GetPin(pScale, L"DXT Input 0"), NULL);
    CHECK_HR(hr, _T("Can't connect LAV Video Decoder and Scale"));


    //add Color Space Converter
    CComPtr<IBaseFilter> pColorSpaceConverter;
    hr = pColorSpaceConverter.CoCreateInstance(CLSID_Colour);
    CHECK_HR(hr, _T("Can't create Color Space Converter"));
    hr = pGraph->AddFilter(pColorSpaceConverter, L"Color Space Converter");
    CHECK_HR(hr, _T("Can't add Color Space Converter to graph"));


    //connect Scale and Color Space Converter
    hr = pGraph->ConnectDirect(GetPin(pScale, L"DXT Output"), GetPin(pColorSpaceConverter, L"Input"), NULL);
    CHECK_HR(hr, _T("Can't connect Scale and Color Space Converter"));


    //connect Color Space Converter and AJA M-Renderer Kona3G - 0:0
    hr = pGraph->ConnectDirect(GetPin(pColorSpaceConverter, L"XForm Out"), GetPin(pAJARendererKona3G, L"Vid"), NULL);
    CHECK_HR(hr, _T("Can't connect Color Space Converter and AJA Renderer Kona3G"));


    return S_OK;
}

void usage(char* argv0)
{
    printf("AJA CLI Player Usage:\n%s [-m|--multi|-s|--single] [-c|--card <n>] [-d|--dev <n>] [-p|--port <n>] [-f|--file] [filename.ext]\n", argv0);
    printf("\n");
    printf("\t-m\tUse AJAMultiRenderer64.dll\n");
    printf("\t-s\tUse AJARenderer64.dll\n");
    printf("\t-c <0-3>\tAJA Kona Card #\n");
    printf("\t-d <0-1>\tPlayback Engine\n");
    printf("\t-p <0-9>\tOutput Port\n");
    printf("\t      0:\tHDMI\n");
    printf("\t      1:\tANALOG\n");
    printf("\t    2-9:\tSDI 1 - SDI 8\n");
    printf("\n");
    printf("Example:\n\t%s test.avi\n", argv0);
}

//int _tmain(int argc, _TCHAR* argv[]) //use this line in VS2008
int main(int argc, char* argv[])
{
    int i;
    int outType = -1;
    int outCard = -1;
    int outDevice = -1;
    int outPort = -1;
    std::string strFile;

    CoInitialize(NULL);
    CComPtr<IGraphBuilder> graph;
    graph.CoCreateInstance(CLSID_FilterGraph);

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") || !strcmp(argv[i], "/?")) {
            usage(argv[0]);
            return 0;
        } else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--multi")) {
            if (outType != -1) {
                printf("Error! multiple output types specified.\n");
                return -1;
            }
            outType = 1;
        } else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--single")) {
            if (outType != -1) {
                printf("Error! multiple output types specified.\n");
                return -1;
            }
            outType = 0;
        } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--card")) {
            i++;
            if(outCard != -1) {
                printf("Error! multiple cards specified.\n");
                return -1;
            } else if (i < argc) {
                outCard = strtol(argv[i], NULL, 0);
            } else {
                printf("Error! no card specified.\n");
                return -1;
            }
            if ((outCard < 0) || (outCard > 3)) {
                printf("Error! card specified outside range.\n");
                return -1;
            }
        } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dev")) {
            i++;
            if(outDevice != -1) {
                printf("Error! multiple devices specified.\n");
                return -1;
            } else if (i < argc) {
                outDevice = strtol(argv[i], NULL, 0);
            } else {
                printf("Error! no device specified.\n");
                return -1;
            }
            if ((outDevice < 0) || (outDevice > 3)) {
                printf("Error! device specified outside range.\n");
                return -1;
            }
        } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
            i++;
            if (outPort != -1) {
                printf("Error! multiple ports specified.\n");
                return -1;
            }
            else if (i < argc) {
                outPort = strtol(argv[i], NULL, 0);
            }
            else {
                printf("Error! no port specified.\n");
                return -1;
            }
            if ((outPort < 0) || (outPort > 5)) {
                printf("Error! port specified outside range.\n");
                return -1;
            }
        } else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) {
            i++;
            if (!strFile.empty()) {
                printf("Error! multiple files specified.\n");
                return -1;
            } else if (i < argc) {
                strFile = argv[i];
            }
            else {
                printf("Error! no file specified.\n");
                return -1;
            }
        } else if (argv[i][0] == '-') {
            printf("Error! unknown argument '%s'.\n", argv[i] );
            return -1;
        } else {
            if (!strFile.empty()) {
                printf("Error! multiple files specified.\n");
                return -1;
            } else {
                strFile = argv[i];
            }
        }
    }

    if (outType == -1) {
        outType = 0;
    }

    if (outCard == -1) {
        outCard = 0;
    }

    if (outDevice == -1) {
        outDevice = 0;
    }

    if (strFile.empty()) {
        printf("Error! no file specified.\n");
        return -1;
    }

    _bstr_t bstrFile = strFile.c_str();

    printf("Opening '%s'\n", strFile.c_str());

    //printf("Building graph...\n");
    HRESULT hr = BuildGraph(graph, outType, outCard, outDevice, outPort, bstrFile);
    if (hr == S_OK) {
        printf("PLAY\n");
        CComQIPtr<IMediaControl, &IID_IMediaControl> mediaControl(graph);
        hr = mediaControl->Run();
        CHECK_HR(hr, _T("Can't run the graph"));
        CComQIPtr<IMediaEvent, &IID_IMediaEvent> mediaEvent(graph);
        BOOL stop = FALSE;
        MSG msg;
        while (!stop)
        {
            long ev = 0;
            LONG_PTR p1 = 0, p2 = 0;
            Sleep(500);
            //printf(".");
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                DispatchMessage(&msg);
            while (mediaEvent->GetEvent(&ev, &p1, &p2, 0) == S_OK)
            {
                if (ev == EC_COMPLETE || ev == EC_USERABORT)
                {
                    printf("STOP\n");
                    stop = TRUE;
                }
                else
                    if (ev == EC_ERRORABORT)
                    {
                        printf("An error occured: HRESULT=%llx\n", p1);
                        mediaControl->Stop();
                        stop = TRUE;
                    }
                mediaEvent->FreeEventParams(ev, p1, p2);
            }
        }
    }
    CoUninitialize();
    return 0;
}
