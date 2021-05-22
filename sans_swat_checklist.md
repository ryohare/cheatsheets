# Defending Web Applications Checklist
Source material from [here](https://www.sans.org/sites/default/files/2018-01/STH-poster-winter-2013.pdf)
- [Defending Web Applications Checklist](#defending-web-applications-checklist)
  * [Error Handling and Logging](#error-handling-and-logging)
  * [Data Protection](#data-protection)
  * [Authentication](#authentication)
  * [Input and Output Handling](#input-and-output-handling)
  * [Configuration and Operations](#configuration-and-operations)
  * [Session Management](#session-management)
  * [Access Control](#access-control)
## Error Handling and Logging
- [ ] Display generic error messages
- [ ] No unhandled exceptions
- [ ] Surpress framework generated errors
- [ ] Log all authentication activities
- [ ] Log all privilege changes
- [ ] Log all administrative activities
- [ ] Log access to sensitive data
- [ ] Do not log sensitive data
- [ ] Store logs securely
## Data Protection
- [ ] Use TLS Everywhere
- [ ] Disable HTTP access and force all connections to HTTPS
- [ ] Use `Strict-Transport-Sercurity` header
- [ ] Store user passwords using a strong, iterative, salted hash
- [ ] Securely exchange encryption keys
- [ ] Setup secure key management process
- [ ] If TLS 1.2, disable weak Cipher Suites
- [ ] Use certificates from reputable valid root CA
- [ ] Disable data caching using cache control headers and disabling autocomplete
- [ ] Limit use of client side storage
- [ ] Do no store sensitive or confidential data in client side storage
## Authentication
- [ ] Don't hardcode credentials
- [ ] Develop a strong password reset system
- [ ] Implement a strong password policy
- [ ] Implement account lockout to prevent brute force attacks
- [ ] Don't disclose too much information in error messages
- [ ] Store database credentials securely
- [ ] Applications and middleware should run with minimal permissions
## Input and Output Handling
- [ ] Conduct contexual output encdoing
- [ ] Prefer whitelists over blacklists
- [ ] Use parameterized SQL queries
- [ ] Use an object relational mapper over SQL queries
- [ ] Use tokens to prevent forged requests
- [ ] Set the encoding for the application
- [ ] Validate uploaded files
- [ ] Use the `nosniff` header for uploaded content
- [ ] Validate the source of input
- [ ] Use the `X-Frame-Options` header
- [ ] Use Content Security Policy `X-XSS` protection headers
## Configuration and Operations
- [ ] Establish a rigorous change management process
- [ ] Define security requirements
- [ ] Conduct a design review
- [ ] Perform a code review
- [ ] perform security testing
- [ ] Harden the infrastructure
- [ ] Define an incident handling plan
- [ ] Educate the team on securty
## Session Management
- [ ] Ensure that session identifiers are sufficiently random
- [ ] Regenerate session tokens on each request
- [ ] Implement and idle session timeout
- [ ] Implement an absolutes session timeout
- [ ] Destroy sessions at any sign of tampering
- [ ] Invalidate the session after logout
- [ ] Place a logout button on every page
- [ ] Use secure cookie attributes
- [ ] Set the cookie domain and path correctly
- [ ] Set the cookie expiration time
## Access Control
- [ ] Apply access control checks consistently
- [ ] Apply the principal of least privilege
- [ ] Don't sure direct object references for access control checks
- [ ] Don't use unvalidated forwards or redirects