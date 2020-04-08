
/*++

Copyright (C) 2019 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

NMR_ModelReaderNode_KeyStoreResourceDataGroup.h defines the Model Reader Node class that is related to <resourcedatagroup>.

--*/

#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreResourceDataGroup.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreAccessRight.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreResourceData.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReaderNode_KeyStoreResourceDataGroup::CModelReaderNode_KeyStoreResourceDataGroup(CKeyStore * pKeyStore, PModelReaderWarnings pWarnings)
		: CModelReaderNode_KeyStoreBase(pKeyStore, pWarnings) {}

	PKeyStoreResourceDataGroup CModelReaderNode_KeyStoreResourceDataGroup::getResourceDataGroup()
	{
		return m_pResourceDataGroup;
	}

	void CModelReaderNode_KeyStoreResourceDataGroup::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		m_pResourceDataGroup = std::make_shared<CKeyStoreResourceDataGroup>(m_keyUUID, m_accessRights, m_resourcesData);

	}

	void CModelReaderNode_KeyStoreResourceDataGroup::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(XML_3MF_SECURE_CONTENT_KEY_UUID, pAttributeName) == 0) {
			if (!m_keyUUID)
				m_pWarnings->addException(CNMRException(NMR_ERROR_KEYSTOREINVALIDKEYUUID), eModelReaderWarningLevel::mrwInvalidMandatoryValue);
			m_keyUUID = std::make_shared<CUUID>(pAttributeValue);
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_KeyStoreResourceDataGroup::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_SECURECONTENTSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_RESOURCEDATA) == 0) {
				PModelReaderNode_KeyStoreResourceData pXMLNode = std::make_shared<CModelReaderNode_KeyStoreResourceData>(m_pKeyStore, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				PKeyStoreResourceData rd = pXMLNode->getResourceData();
				if (nullptr != rd) {
					m_resourcesData.push_back(rd);
				}
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_ACCESSRIGHT) == 0) {
				PModelReaderNode_KeyStoreAccessRight pXMLNode = std::make_shared<CModelReaderNode_KeyStoreAccessRight>(m_pKeyStore, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				PKeyStoreAccessRight ar = pXMLNode->getAccessRight();
				if (nullptr != ar) {
					m_accessRights.push_back(ar);
				}
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
