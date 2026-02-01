<template>
	<view class="wrap">
		<view class="dev-area">

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">温度 </view>
					<image class="dev-logo" src="../../static/tem.png" mode=""></image>
				</view>
				<view class="dev-data">{{temp}} ℃</view>
			</view>
			<view class="dev-cart">
				<view class="">
					<view class="dev-name">湿度 </view>
					<image class="dev-logo" src="../../static/humi.png" mode=""></image>
				</view>
				<view class="dev-data">{{humi}} ℃</view>
			</view>
			<view class="dev-cart">
				<view class="">
					<view class="dev-name">台灯 </view>
					<image class="dev-logo" src="../../static/light.png" mode=""></image>
				</view>
				<switch :checked="led" @change="onLedSwitch" color="#2b9939" />
			</view>





			<!-- <view class="">湿度{{humi}} %</view>
	<switch :checked="led" @change="onLedSwitch" /> -->
		</view>
	</view>

</template>

<script>
	import {
		version
	} from 'os'

	const {
		createCommonToken
	} = require('@/key.js')
	export default {
		data() {
			return {
				temp: '',
				humi: '',
				led: true,
				token: '',
			}
		},
		onLoad() {
			const params = {
				access_key: 'X70tWc/xES8FM0VrZd3X7aNjW5HtOdOZP4lLmRGh0',
				version: '2022-05-01',
				productid: 'gf0B67Ey48',
			}

			this.token = createCommonToken(params);
		},
		onShow() {
			this.fetchDevData();
			setInterval(()=>{
				this.fetchDevData();
			},3000)

		},
		methods: {
			fetchDevData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property',
					method: 'GET',
					data: {
						product_id: 'gf0B67Ey48',
						device_name: 'd1',
					},
					header: {
						'authorization': 'version=2018-10-31&res=products%2Fgf0B67Ey48%2Fdevices%2Fd1&et=2053588735&method=md5&sign=DU1iAf1hfnoZ2aazh6Krug%3D%3D' //自定义请求头信息
					},
					success: (res) => {
						console.log(res.data);
						this.humi = res.data.data[0].value;
						this.led = res.data.data[1].value === 'true';
						this.temp = res.data.data[2].value;
					}
				});

			},

			onLedSwitch(event) {
				console.log(event.detail.value);
				let value = event.detail.value;
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: {
						product_id: 'gf0B67Ey48',
						device_name: 'd1',
						params: {
							"led": value
						}
					},
					header: {
						'authorization': 'version=2018-10-31&res=products%2Fgf0B67Ey48%2Fdevices%2Fd1&et=2053588735&method=md5&sign=DU1iAf1hfnoZ2aazh6Krug%3D%3D' //自定义请求头信息
					},
					success: () => {
						console.log('LED ' + (value ? 'ON' : 'OFF') + ' !');

					}
				});

			}


		}
	}
</script>

<style>
	.wrap {
		padding: 30rpx;
	}

	.dev-area {
		display: flex;
		justify-content: space-between;
		flex-wrap: wrap;
		gap: 30rpx;
	}

	.dev-cart {
		height: 150rpx;
		width: 320rpx;
		border-radius: 30rpx;
		margin-top: 30px;
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
	}

	.dev-name {
		font-size: 20rpx;
		text-align: center;
		color: #6d6d6d;

	}

	.dev-logo {
		width: 70rpx;
		height: 70rpx;
		margin-top: 10rpx;
	}

	.dev-data {
		font-size: 50rpx;

		color: #6d6d6d;


	}


	
</style>
