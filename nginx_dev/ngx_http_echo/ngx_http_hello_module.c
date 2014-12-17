/* 
 * File:   ngx_http_hello_module.c
 * Author: chenqiuyu
 *
 * Created on 2014年11月8日, 下午1:45
 */

static ngx_int_t gnx_http_mytest_handler(ngx_http_request_t *r){
	
	//必须是GET或HEAD方法，否则返回405 Not Allowed
	if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))){
		return NGX_HTTP_NOT_ALLOWED;
	}
	//丢弃请求中的包体
	ngx_int_t rc = ngx_http_discard_request_body(r);
	if (rc != NGX_OK){
		return rc;
	}

	/*设置返回的Content-Type。注意，ngx_str_t有一个很方便的初始化宏ngx_string，他可以把ngx_str_t的data和len成员都设置好*/
	ngx_str_t type = ngx_string("text/plain");
	//返回的包体内容
	ngx_str_t response = ngx_string("Hello World!");
	//设置返回的状态码
	r->headers_out.status = NGX_HTTP_OK;
	//响应包是有包体内容的，需要设置Content-Length长度
	r->headers_out.content_length_n = response.len;
	//设置Content-Type
	r->headers_out.content_type = type;

	//发送HTTP头部
	rc = ngx_http_send_header(r);
	if(rc == NGX_ERROR || rc > NGX_OK) || r->header_only){
		return rc;
	}

	//构建ngx_buf_t结构体准备发送包体
	ngx_buf_t *b;
	b = ngx_create_temp_buf(r->pool, response.len);
	if(b == NULL){
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}
	//将Hello World复制到ngx_buf_t指向的内存中
	ngx_memcpy(b->pos, response.len);
	//注意，一定要设置好last指针

}

