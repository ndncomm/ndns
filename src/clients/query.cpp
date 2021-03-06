/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014, Regents of the University of California.
 *
 * This file is part of NDNS (Named Data Networking Domain Name Service).
 * See AUTHORS.md for complete list of NDNS authors and contributors.
 *
 * NDNS is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NDNS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NDNS, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "query.hpp"

namespace ndn {
namespace ndns {

Query::Query()
  : m_interestLifetime(ndn::DEFAULT_INTEREST_LIFETIME)
{
}

Query::Query(const Name& hint, const Name& zone, const name::Component& queryType)
  : m_hint(hint)
  , m_zone(zone)
  , m_queryType(queryType)
  , m_interestLifetime(ndn::DEFAULT_INTEREST_LIFETIME)
{
}


bool
Query::fromInterest(const Name& hint, const Name& zone, const Interest& interest)
{
  label::MatchResult re;
  if (!matchName(interest, hint, zone, re))
    return false;

  m_rrLabel = re.rrLabel;
  m_rrType = re.rrType;

  m_hint = hint;
  m_zone = zone;

  size_t len = zone.size();
  if (!hint.empty())
    len += hint.size() + 1;
  m_queryType = interest.getName().get(len);

  return true;
}

Interest
Query::toInterest() const
{
  // <hint> /xF0. <zone> [<KEY>|<NDNS>|<NDNS-R>] <rrLabel> <rrType>
  Name name;
  if (!m_hint.empty()) {
    name.append(m_hint)
        .append(ndn::ndns::label::FORWARDING_HINT_LABEL);
  }

  name.append(this->m_zone)
      .append(this->m_queryType)
      .append(this->m_rrLabel)
      .append(this->m_rrType);

  return Interest(name, m_interestLifetime);
}

std::ostream&
operator<<(std::ostream& os, const Query& query)
{
  os << "Query: zone=" << query.getZone()
     << " hint=" << query.getHint()
     << " queryType=" << query.getQueryType()
     << " rrLabel=" << query.getRrLabel()
     << " rrType=" << query.getRrType()
     << " Lifetime=" << query.getInterestLifetime()
    ;
  return os;
}

} // namespace ndns
} // namespace ndn
