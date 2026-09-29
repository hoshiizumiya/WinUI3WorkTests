#include "pch.h"
#include "TaskSampleViewModel.h"
#if __has_include("TaskSampleViewModel.g.cpp")
#include "TaskSampleViewModel.g.cpp"
#endif

namespace winrt::WinUI3cppWorkTest::implementation
{
    TaskSampleViewModel::TaskSampleViewModel(winrt::hstring const& name, int32_t index)
        : m_name(name),
          m_size((index % 2) == 0 ? L"1.42 GB" : L"824 MB"),
          m_progress((index % 3) == 0 ? L"38.0%" : L"72.5%"),
          m_downloadSize((index % 2) == 0 ? L"542 MB" : L"598 MB"),
          m_uploadSize((index % 2) == 0 ? L"18.2 MB" : L"0 B"),
          m_totalDownloadSize((index % 2) == 0 ? L"542 MB" : L"598 MB"),
          m_totalUploadSize((index % 2) == 0 ? L"18.2 MB" : L"0 B"),
          m_downloadRate((index % 2) == 0 ? L"4.2 MB/s" : L"2.8 MB/s"),
          m_uploadRate((index % 2) == 0 ? L"128 KB/s" : L"0 B/s"),
          m_remaining((index % 3) == 0 ? L"3 min" : L"1 min"),
          m_addDate(L"2026/09/29 12:34"),
          m_completedDate(L"-"),
          m_shareRatio((index % 2) == 0 ? L"0.03" : L"-"),
          m_seeds((index % 2) == 0 ? L"18 / 42" : L"-"),
          m_transport((index % 2) == 0 ? L"BitTorrent · Downloading" : L"HTTP · Downloading"),
          m_progressPercent((index % 3) == 0 ? 38.0 : 72.5),
          m_progressHighColor((index % 2) == 0
              ? winrt::Windows::UI::Color{ 255, 235, 70, 70 }
              : winrt::Windows::UI::Color{ 255, 76, 175, 80 })
    {
    }
}
