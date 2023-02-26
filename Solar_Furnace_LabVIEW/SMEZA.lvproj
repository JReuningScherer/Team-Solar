<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="22308000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">true</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="smeza_labview.vi" Type="VI" URL="../../smeza_labview.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Board Type.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Interface for Arduino/Type Defs/Board Type.ctl"/>
				<Item Name="Close.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Interface for Arduino/Close.vi"/>
				<Item Name="Connection Type.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Interface for Arduino/Type Defs/Connection Type.ctl"/>
				<Item Name="Digital Write Pin.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Interface for Arduino/Low Level/Digital Write Pin.vi"/>
				<Item Name="Init.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Interface for Arduino/Init.vi"/>
				<Item Name="Pin Mode.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Interface for Arduino/Type Defs/Pin Mode.ctl"/>
				<Item Name="Set Digital Pin Mode.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Interface for Arduino/Low Level/Set Digital Pin Mode.vi"/>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
