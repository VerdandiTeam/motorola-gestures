Name:       motorola-gestures

Summary:    Gestures daemon
Version:    0.1
Release:    1
License:    LICENSE
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(mlite5)
BuildRequires:  desktop-file-utils

%description
Gestures daemon

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5 
%make_build

%install
%qmake5_install

%preun
if [ "$1" = "0" ]; then
  systemctl-user stop motorola-gestures || true
  systemctl-user disable motorola-gestures || true
fi

%post
systemctl-user daemon-reload || true
systemctl-user start motorola-gestures || true
systemctl-user enable motorola-gestures || true

%pre
# In case of update, stop first
if [ "$1" = "2" ]; then
  systemctl-user stop motorola-gestures || true
  systemctl-user disable motorola-gestures || true
fi

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/jolla-settings/entries/
%{_datadir}/jolla-settings/pages/motorola-gestures/
%{_datadir}/translations
/usr/lib/systemd/user/
